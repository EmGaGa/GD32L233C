#include "header.h"

static void rcu_config(void);
static void adc_config(void);
static void gpio_config(void);
static uint16_t ADC_Get_Channel(uint8_t channel);
/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void rcu_config(void)
{ 
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void adc_config(void)
{
    /* ADC data alignment config */
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC_REGULAR_CHANNEL, 1U);

    /* ADC trigger config */
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
    /* ADC external trigger config */
    adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

    /* enable ADC interface */
    adc_enable();
    delay_1ms(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable();
}

static void gpio_config(void)
{
    rcu_periph_clock_enable(ROTATION_SENSOR_GPIO_RCU);  
    /* PB0 ADC_IN config */
    gpio_mode_set(ROTATION_SENSOR_GPIO_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ROTATION_SENSOR_GPIO_PIN);
}

static uint16_t ADC_Get_Channel(uint8_t channel)
{
    /* ADC regular channel config */
    adc_regular_channel_config(0U, channel, ADC_SAMPLETIME_239POINT5);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC_REGULAR_CHANNEL);

    /* wait the end of conversion flag */
    while(!adc_flag_get(ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_regular_data_read());   
}

void rotationSensorInit(void)
{
    gpio_config();
    rcu_config();
    adc_config();
}

uint16_t getRotaSensorVar(void)
{
    uint16_t adcval;

    adcval = ADC_Get_Channel(ROTATION_ADC_CH);
    if(adcval < 145)
        return 0x00;
    else if(adcval > 3970)
        return 0xff;
    else
        return (adcval-145)/15;
}
