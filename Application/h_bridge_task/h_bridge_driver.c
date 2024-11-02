/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "app.h"

#include "h_bridge_driver.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define SD_DUTY_MIN \
(APB1_TIMER_CLK * 50) / (H_Bridge_x->PWM.Prescaler * 1000000)
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
__STATIC_INLINE void HB_Set_Duty(PWM_TypeDef *PWMx, uint32_t _Duty, bool apply_now);
__STATIC_INLINE void HB_Set_OC(PWM_TypeDef *PWMx, uint32_t _OC, bool apply_now);
__STATIC_INLINE void HB_Set_Freq(PWM_TypeDef *PWMx, float _Freq, bool apply_now);
__STATIC_INLINE void HB_Set_ARR(PWM_TypeDef *PWMx, uint32_t _ARR, bool apply_now);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint32_t HB_PWM_channel_array[8] =
{
    H_BRIDGE_SD0_CHANNEL,
    H_BRIDGE_SD1_CHANNEL,
    H_BRIDGE_SD2_CHANNEL,
    H_BRIDGE_SD3_CHANNEL,
    H_BRIDGE_SD4_CHANNEL,
    H_BRIDGE_SD5_CHANNEL,
    H_BRIDGE_SD6_CHANNEL,
    H_BRIDGE_SD7_CHANNEL,
};

PWM_TypeDef HB_PWM_SD0_3 =
{
    .TIMx       =   H_BRIDGE_SD0_3_HANDLE,
    .Prescaler  =   1199,
    .Mode       =   LL_TIM_OCMODE_FORCED_ACTIVE,
    .Polarity   =   LL_TIM_OCPOLARITY_HIGH,
    .Duty       =   3, //100us
    .Freq       =   0,
};

PWM_TypeDef HB_PWM_SD4_7 =
{
    .TIMx       =   H_BRIDGE_SD4_7_HANDLE,
    .Prescaler  =   1199,
    .Mode       =   LL_TIM_OCMODE_FORCED_ACTIVE,
    .Polarity   =   LL_TIM_OCPOLARITY_HIGH,
    .Duty       =   3, //100us
    .Freq       =   0,
};

uint32_t HB_pin_array[8] =
{
    H_BRIDGE_HIN0_PIN,
    H_BRIDGE_HIN1_PIN,
    H_BRIDGE_HIN2_PIN,
    H_BRIDGE_HIN3_PIN,
    H_BRIDGE_HIN4_PIN,
    H_BRIDGE_HIN5_PIN,
    H_BRIDGE_HIN6_PIN,
    H_BRIDGE_HIN7_PIN,
};

bool HB_pin_state_array[8] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

H_Bridge_typdef HB_pos_pole =
{
    .Port               = H_BRIDGE_HIN0_7_PORT,
    .Mode               = H_BRIDGE_MODE_LS_ON,
    .delay_time_ms      = 0,
    .on_time_ms         = 0,
    .off_time_ms        = 0,
    .set_pulse_count    = 0,
    .pulse_count        = 0,
    .is_setted          = false,
};

H_Bridge_typdef HB_neg_pole =
{
    .Port               = H_BRIDGE_HIN0_7_PORT,
    .Mode               = H_BRIDGE_MODE_LS_ON,
    .delay_time_ms      = 0,
    .on_time_ms         = 0,
    .off_time_ms        = 0,
    .set_pulse_count    = 0,
    .pulse_count        = 0,
    .is_setted          = false,
};

H_Bridge_typdef *p_HB_SD_0_3 = &HB_pos_pole;
H_Bridge_typdef *p_HB_SD_4_7 = &HB_neg_pole;

uint8_t HB_pos_pole_index = 0;
uint8_t HB_neg_pole_index = 4;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: H Bridge Driver Init :::::::: */
void H_Bridge_Driver_Init(void)
{   
    for (uint8_t i = 0; i < 4; i++)
    {
        LL_TIM_OC_SetMode(HB_PWM_SD0_3.TIMx, HB_PWM_channel_array[i],  HB_PWM_SD0_3.Mode);
        LL_TIM_OC_SetPolarity(HB_PWM_SD0_3.TIMx, HB_PWM_channel_array[i], HB_PWM_SD0_3.Polarity);
        LL_TIM_OC_EnablePreload(HB_PWM_SD0_3.TIMx, HB_PWM_channel_array[i]);
        LL_TIM_CC_EnableChannel(HB_PWM_SD0_3.TIMx, HB_PWM_channel_array[i]);
        LL_GPIO_ResetOutputPin(H_BRIDGE_HIN0_7_PORT, HB_pin_array[i]);
    }

    for (uint8_t i = 4; i < 8; i++)
    {
        LL_TIM_OC_SetMode(HB_PWM_SD4_7.TIMx, HB_PWM_channel_array[i],  HB_PWM_SD4_7.Mode);
        LL_TIM_OC_SetPolarity(HB_PWM_SD4_7.TIMx, HB_PWM_channel_array[i], HB_PWM_SD4_7.Polarity);
        LL_TIM_OC_EnablePreload(HB_PWM_SD4_7.TIMx, HB_PWM_channel_array[i]);
        LL_TIM_CC_EnableChannel(HB_PWM_SD4_7.TIMx, HB_PWM_channel_array[i]);
        LL_GPIO_ResetOutputPin(H_BRIDGE_HIN0_7_PORT, HB_pin_array[i]);
    }

    LL_TIM_SetPrescaler(H_BRIDGE_SD0_3_HANDLE, 1199);
    LL_TIM_SetPrescaler(H_BRIDGE_SD4_7_HANDLE, 1199);

    LL_TIM_EnableARRPreload(H_BRIDGE_SD0_3_HANDLE);
    LL_TIM_EnableARRPreload(H_BRIDGE_SD4_7_HANDLE);

    LL_TIM_SetUpdateSource(H_BRIDGE_SD0_3_HANDLE, LL_TIM_UPDATESOURCE_REGULAR);
    LL_TIM_SetUpdateSource(H_BRIDGE_SD4_7_HANDLE, LL_TIM_UPDATESOURCE_REGULAR);

    LL_TIM_DisableIT_UPDATE(H_BRIDGE_SD0_3_HANDLE);
    LL_TIM_GenerateEvent_UPDATE(H_BRIDGE_SD0_3_HANDLE);

    LL_TIM_DisableIT_UPDATE(H_BRIDGE_SD4_7_HANDLE);
    LL_TIM_GenerateEvent_UPDATE(H_BRIDGE_SD4_7_HANDLE);

    LL_TIM_EnableCounter(H_BRIDGE_SD0_3_HANDLE);
    LL_TIM_EnableCounter(H_BRIDGE_SD4_7_HANDLE);
}

void H_Bridge_Set_Pole(void)
{
    HB_pos_pole.Pin         = &HB_pin_array[HB_pos_pole_index];
    HB_pos_pole.Pin_State   = &HB_pin_state_array[HB_pos_pole_index];

    (HB_pos_pole_index < 4) ? (HB_pos_pole.PWM = HB_PWM_SD0_3) : (HB_pos_pole.PWM = HB_PWM_SD4_7);
    HB_pos_pole.PWM.Channel = HB_PWM_channel_array[HB_pos_pole_index];

    HB_neg_pole.Pin         = &HB_pin_array[HB_neg_pole_index];
    HB_neg_pole.Pin_State   = &HB_pin_state_array[HB_neg_pole_index];
    
    (HB_neg_pole_index < 4) ? (HB_neg_pole.PWM = HB_PWM_SD0_3) : (HB_neg_pole.PWM = HB_PWM_SD4_7);
    HB_neg_pole.PWM.Channel = HB_PWM_channel_array[HB_neg_pole_index];
}

void H_Bridge_Set_Mode(H_Bridge_typdef* H_Bridge_x, H_Bridge_mode SetMode)
{
    LL_TIM_DisableIT_UPDATE(H_Bridge_x->PWM.TIMx);
    LL_TIM_DisableCounter(H_Bridge_x->PWM.TIMx);
    LL_TIM_ClearFlag_UPDATE(H_Bridge_x->PWM.TIMx);
    (H_Bridge_x->PWM.TIMx ==  H_BRIDGE_SD0_3_HANDLE) ? (p_HB_SD_0_3 = H_Bridge_x) : (p_HB_SD_4_7 = H_Bridge_x);
    H_Bridge_x->Mode = SetMode;

    switch (SetMode)
    {
    case H_BRIDGE_MODE_PULSE:
        HB_Set_Freq(&H_Bridge_x->PWM, (1000.0 / (float)H_Bridge_x->delay_time_ms), 1);
        HB_Set_OC(&H_Bridge_x->PWM, 0, 1);
        LL_TIM_OC_SetMode(H_Bridge_x->PWM.TIMx, H_Bridge_x->PWM.Channel, LL_TIM_OCMODE_PWM2);
        HB_Set_Freq(&H_Bridge_x->PWM, (1000.0 / (float)H_Bridge_x->on_time_ms), 0); //Period = 1ms
        HB_Set_OC(&H_Bridge_x->PWM, H_Bridge_x->PWM.Duty, 0); //Duty = 50us
        
        *H_Bridge_x->Pin_State = 1;
        LL_TIM_ClearFlag_UPDATE(H_Bridge_x->PWM.TIMx);

        break;
    case H_BRIDGE_MODE_HS_ON:
    case H_BRIDGE_MODE_LS_ON:
        LL_TIM_OC_SetMode(H_Bridge_x->PWM.TIMx, H_Bridge_x->PWM.Channel, LL_TIM_OCMODE_PWM2);
        HB_Set_Freq(&H_Bridge_x->PWM, 1000.0, 1); //Period = 1ms
        HB_Set_Duty(&H_Bridge_x->PWM, 10, 1); //Duty = 100us
        H_Bridge_x->pulse_count = 0;
        H_Bridge_x->delay_time_ms = 0;

        break;
    case H_BRIDGE_MODE_FLOAT:
        LL_TIM_OC_SetMode(H_Bridge_x->PWM.TIMx, H_Bridge_x->PWM.Channel, LL_TIM_OCMODE_FORCED_INACTIVE);
        HB_Set_ARR(&H_Bridge_x->PWM, 0, 1); //Period = 1ms
        HB_Set_OC(&H_Bridge_x->PWM, 0, 1); //Duty = 50us
        H_Bridge_x->pulse_count = 0;
        H_Bridge_x->delay_time_ms = 0;

        LL_TIM_ClearFlag_UPDATE(H_Bridge_x->PWM.TIMx);

        break;
    
    default:
        break;
    }

    LL_TIM_EnableCounter(H_Bridge_x->PWM.TIMx);
    LL_TIM_EnableIT_UPDATE(H_Bridge_x->PWM.TIMx);
}

void H_Bridge_Set_Pulse_Timing(H_Bridge_typdef* H_Bridge_x, uint16_t Set_delay_time_ms, uint16_t Set_on_time_ms, uint16_t Set_off_time_ms, uint16_t Set_pulse_count)
{
    LL_TIM_DisableIT_UPDATE(H_Bridge_x->PWM.TIMx);
    LL_TIM_DisableCounter(H_Bridge_x->PWM.TIMx);
    LL_TIM_ClearFlag_UPDATE(H_Bridge_x->PWM.TIMx);

    /* 
    * @brief Calculate the maximum of Set_delay_time_ms, Set_on_time_ms, and Set_off_time_ms.
    * @details 
    *   - If 'Set_delay_time_ms' is greater than 'Set_on_time_ms', check if 'Set_delay_time_ms' is also greater than 'Set_off_time_ms'.
    *     If true, 'Set_delay_time_ms' is the maximum; otherwise, 'Set_off_time_ms' is the maximum.
    *   - If 'Set_delay_time_ms' is not greater than 'Set_on_time_ms', then check if 'Set_on_time_ms' is greater than 'Set_off_time_ms'.
    *     If true, 'Set_on_time_ms' is the maximum; otherwise, 'Set_off_time_ms' is the maximum.
    */
    uint16_t max_time_ms =  (Set_delay_time_ms > Set_on_time_ms) ? 
                            ((Set_delay_time_ms > Set_off_time_ms) ? Set_delay_time_ms : Set_off_time_ms) : 
                            ((Set_on_time_ms > Set_off_time_ms) ? Set_on_time_ms : Set_off_time_ms);

    uint16_t min_time_ms =  (Set_on_time_ms < Set_off_time_ms) ? Set_on_time_ms : Set_off_time_ms;
     
    H_Bridge_x->PWM.Prescaler   = ((APB1_TIMER_CLK * max_time_ms) / (UINT16_MAX * 1000)) + 1;
    H_Bridge_x->PWM.Prescaler   = (H_Bridge_x->PWM.Prescaler > UINT16_MAX) ? UINT16_MAX : H_Bridge_x->PWM.Prescaler;

    H_Bridge_x->PWM.Duty        = (APB1_TIMER_CLK * min_time_ms) / (H_Bridge_x->PWM.Prescaler * 100 * 1000);
    H_Bridge_x->PWM.Duty        = (H_Bridge_x->PWM.Duty > SD_DUTY_MIN) ? H_Bridge_x->PWM.Duty : SD_DUTY_MIN;

    H_Bridge_x->delay_time_ms   = Set_delay_time_ms;

    H_Bridge_x->on_time_ms      = Set_on_time_ms;
    H_Bridge_x->off_time_ms     = Set_off_time_ms;

    H_Bridge_x->set_pulse_count = Set_pulse_count;
    H_Bridge_x->pulse_count     = 0;

    LL_TIM_SetPrescaler(H_Bridge_x->PWM.TIMx, H_Bridge_x->PWM.Prescaler);
    LL_TIM_GenerateEvent_UPDATE(H_Bridge_x->PWM.TIMx);
    LL_TIM_ClearFlag_UPDATE(H_Bridge_x->PWM.TIMx);

    LL_TIM_EnableIT_UPDATE(H_Bridge_x->PWM.TIMx);
    LL_TIM_EnableCounter(H_Bridge_x->PWM.TIMx);
}

void H_Bridge_Kill(void)
{
    H_Bridge_Set_Mode(&HB_pos_pole, H_BRIDGE_MODE_FLOAT);
    H_Bridge_Set_Mode(&HB_neg_pole, H_BRIDGE_MODE_FLOAT);
}

/* ::::H_Bridge SD0_3 Interupt Handle:::: */
void H_Bridge_SD0_3_Interupt_Handle()
{
    if(LL_TIM_IsActiveFlag_UPDATE(H_BRIDGE_SD0_3_HANDLE) == true)
    {
        LL_TIM_ClearFlag_UPDATE(H_BRIDGE_SD0_3_HANDLE);

        switch (p_HB_SD_0_3->Mode)
        {
        case H_BRIDGE_MODE_PULSE:
            p_HB_SD_0_3->pulse_count++;

            if (p_HB_SD_0_3->pulse_count >= ((p_HB_SD_0_3->set_pulse_count * 2) + 1))
            {
                LL_TIM_DisableCounter(H_BRIDGE_SD0_3_HANDLE);
                return;
            }
            
            if (*p_HB_SD_0_3->Pin_State == 1)
            {
                LL_GPIO_SetOutputPin(p_HB_SD_0_3->Port, *p_HB_SD_0_3->Pin);
                HB_Set_Freq(&p_HB_SD_0_3->PWM, 1000.0 / (float)p_HB_SD_0_3->off_time_ms, 0);
                *p_HB_SD_0_3->Pin_State = 0;
            }
            else
            {
                LL_GPIO_ResetOutputPin(p_HB_SD_0_3->Port, *p_HB_SD_0_3->Pin);
                HB_Set_Freq(&p_HB_SD_0_3->PWM, 1000.0 / (float)p_HB_SD_0_3->on_time_ms, 0);
                *p_HB_SD_0_3->Pin_State = 1;
            }

            break;
        case H_BRIDGE_MODE_HS_ON:
            LL_GPIO_SetOutputPin(p_HB_SD_0_3->Port, *p_HB_SD_0_3->Pin);

            LL_TIM_OC_SetMode(p_HB_SD_0_3->PWM.TIMx, p_HB_SD_0_3->PWM.Channel, LL_TIM_OCMODE_FORCED_ACTIVE);
            LL_TIM_GenerateEvent_UPDATE(p_HB_SD_0_3->PWM.TIMx);
            LL_TIM_ClearFlag_UPDATE(p_HB_SD_0_3->PWM.TIMx);
            LL_TIM_DisableIT_UPDATE(p_HB_SD_0_3->PWM.TIMx);
            break;
        case H_BRIDGE_MODE_LS_ON:
            LL_GPIO_ResetOutputPin(p_HB_SD_0_3->Port, *p_HB_SD_0_3->Pin);

            LL_TIM_OC_SetMode(p_HB_SD_0_3->PWM.TIMx, p_HB_SD_0_3->PWM.Channel, LL_TIM_OCMODE_FORCED_ACTIVE);
            LL_TIM_GenerateEvent_UPDATE(p_HB_SD_0_3->PWM.TIMx);
            LL_TIM_ClearFlag_UPDATE(p_HB_SD_0_3->PWM.TIMx);
            LL_TIM_DisableIT_UPDATE(p_HB_SD_0_3->PWM.TIMx);
            break;
        case H_BRIDGE_MODE_FLOAT:
            LL_TIM_DisableIT_UPDATE(p_HB_SD_0_3->PWM.TIMx);
            break;
        
        default:
            break;
        }

        p_HB_SD_0_3->is_setted = true;
    }
}

/* ::::H_Bridge SD4_7 Interupt Handle:::: */
void H_Bridge_SD4_7_Interupt_Handle()
{
    if(LL_TIM_IsActiveFlag_UPDATE(p_HB_SD_4_7->PWM.TIMx) == true)
    {
        LL_TIM_ClearFlag_UPDATE(p_HB_SD_4_7->PWM.TIMx);

        switch (p_HB_SD_4_7->Mode)
        {
        case H_BRIDGE_MODE_PULSE:
            p_HB_SD_4_7->pulse_count++;

            if (p_HB_SD_4_7->pulse_count >= ((p_HB_SD_4_7->set_pulse_count * 2) + 1))
            {
                LL_TIM_DisableCounter(H_BRIDGE_SD0_3_HANDLE);
                return;
            }

            if (*p_HB_SD_4_7->Pin_State == 1)
            {
                LL_GPIO_SetOutputPin(p_HB_SD_4_7->Port, *p_HB_SD_4_7->Pin);
                HB_Set_Freq(&p_HB_SD_4_7->PWM, 1000.0 / (float)p_HB_SD_4_7->off_time_ms, 0);
                *p_HB_SD_4_7->Pin_State = 0;
            }
            else
            {
                LL_GPIO_ResetOutputPin(p_HB_SD_4_7->Port, *p_HB_SD_4_7->Pin);
                HB_Set_Freq(&p_HB_SD_4_7->PWM, 1000.0 / (float)p_HB_SD_4_7->on_time_ms, 0);
                *p_HB_SD_4_7->Pin_State = 1;
            }

            break;
        case H_BRIDGE_MODE_HS_ON:
            LL_GPIO_SetOutputPin(p_HB_SD_4_7->Port, *p_HB_SD_4_7->Pin);

            LL_TIM_OC_SetMode(p_HB_SD_4_7->PWM.TIMx, p_HB_SD_4_7->PWM.Channel, LL_TIM_OCMODE_FORCED_ACTIVE);
            LL_TIM_GenerateEvent_UPDATE(p_HB_SD_4_7->PWM.TIMx);
            LL_TIM_ClearFlag_UPDATE(p_HB_SD_4_7->PWM.TIMx);
            LL_TIM_DisableIT_UPDATE(p_HB_SD_4_7->PWM.TIMx);
            break;
        case H_BRIDGE_MODE_LS_ON:
            LL_GPIO_ResetOutputPin(p_HB_SD_4_7->Port, *p_HB_SD_4_7->Pin);

            LL_TIM_OC_SetMode(p_HB_SD_4_7->PWM.TIMx, p_HB_SD_4_7->PWM.Channel, LL_TIM_OCMODE_FORCED_ACTIVE);
            LL_TIM_GenerateEvent_UPDATE(p_HB_SD_4_7->PWM.TIMx);
            LL_TIM_ClearFlag_UPDATE(p_HB_SD_4_7->PWM.TIMx);
            LL_TIM_DisableIT_UPDATE(p_HB_SD_4_7->PWM.TIMx);
            break;
        case H_BRIDGE_MODE_FLOAT:
            LL_TIM_DisableIT_UPDATE(p_HB_SD_4_7->PWM.TIMx);
            break;
        
        default:
            break;
        }

        p_HB_SD_4_7->is_setted = true;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
__STATIC_INLINE void HB_Set_Duty(PWM_TypeDef *PWMx, uint32_t _Duty, bool apply_now)
{
    LL_TIM_DisableUpdateEvent(PWMx->TIMx);

    // Limit the duty to 100
    if (_Duty > 100)
      return;

    // Set PWM DUTY for channel 1
    _Duty = (PWMx->Freq * (_Duty / 100.0));
    switch (PWMx->Channel)
    {
    case LL_TIM_CHANNEL_CH1:
        LL_TIM_OC_SetCompareCH1(PWMx->TIMx, _Duty);
        break;
    case LL_TIM_CHANNEL_CH2:
        LL_TIM_OC_SetCompareCH2(PWMx->TIMx, _Duty);
        break;
    case LL_TIM_CHANNEL_CH3:
        LL_TIM_OC_SetCompareCH3(PWMx->TIMx, _Duty);
        break;
    case LL_TIM_CHANNEL_CH4:
        LL_TIM_OC_SetCompareCH4(PWMx->TIMx, _Duty);
        break;

    default:
        break;
    }

    LL_TIM_EnableUpdateEvent(PWMx->TIMx);

    if(apply_now == 1)
    {
        if (LL_TIM_IsEnabledIT_UPDATE(PWMx->TIMx))
        {
            LL_TIM_DisableIT_UPDATE(PWMx->TIMx);
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
            LL_TIM_EnableIT_UPDATE(PWMx->TIMx);
        }
        else
        {
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
        }
    }
}

__STATIC_INLINE void HB_Set_OC(PWM_TypeDef *PWMx, uint32_t _OC, bool apply_now)
{
    LL_TIM_DisableUpdateEvent(PWMx->TIMx);

    // Set PWM DUTY for channel 1
    //PWMx->Duty = _OC;
    switch (PWMx->Channel)
    {
    case LL_TIM_CHANNEL_CH1:
        LL_TIM_OC_SetCompareCH1(PWMx->TIMx, _OC);
        break;
    case LL_TIM_CHANNEL_CH2:
        LL_TIM_OC_SetCompareCH2(PWMx->TIMx, _OC);
        break;
    case LL_TIM_CHANNEL_CH3:
        LL_TIM_OC_SetCompareCH3(PWMx->TIMx, _OC);
        break;
    case LL_TIM_CHANNEL_CH4:
        LL_TIM_OC_SetCompareCH4(PWMx->TIMx, _OC);
        break;

    default:
        break;
    }

    LL_TIM_EnableUpdateEvent(PWMx->TIMx);

    if(apply_now == 1)
    {
        if (LL_TIM_IsEnabledIT_UPDATE(PWMx->TIMx))
        {
            LL_TIM_DisableIT_UPDATE(PWMx->TIMx);
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
            LL_TIM_EnableIT_UPDATE(PWMx->TIMx);
        }
        else
        {
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
        }
    }
}

__STATIC_INLINE void HB_Set_Freq(PWM_TypeDef *PWMx, float _Freq, bool apply_now)
{
    LL_TIM_DisableUpdateEvent(PWMx->TIMx);

    // Set PWM FREQ
    //PWMx->Freq = __LL_TIM_CALC_ARR(APB1_TIMER_CLK, LL_TIM_GetPrescaler(PWMx->TIMx), _Freq);
    PWMx->Freq = (float)APB1_TIMER_CLK / ((float)(LL_TIM_GetPrescaler(PWMx->TIMx) + 1) * _Freq);
    LL_TIM_SetAutoReload(PWMx->TIMx, PWMx->Freq);

    LL_TIM_EnableUpdateEvent(PWMx->TIMx);

    if(apply_now == 1)
    {
        if (LL_TIM_IsEnabledIT_UPDATE(PWMx->TIMx))
        {
            LL_TIM_DisableIT_UPDATE(PWMx->TIMx);
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
            LL_TIM_EnableIT_UPDATE(PWMx->TIMx);
        }
        else
        {
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
        }
    }
}

__STATIC_INLINE void HB_Set_ARR(PWM_TypeDef *PWMx, uint32_t _ARR, bool apply_now)
{
    LL_TIM_DisableUpdateEvent(PWMx->TIMx);

    // Set PWM FREQ
    PWMx->Freq = _ARR;
    LL_TIM_SetAutoReload(PWMx->TIMx, PWMx->Freq);

    LL_TIM_EnableUpdateEvent(PWMx->TIMx);

    if(apply_now == 1)
    {
        if (LL_TIM_IsEnabledIT_UPDATE(PWMx->TIMx))
        {
            LL_TIM_DisableIT_UPDATE(PWMx->TIMx);
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
            LL_TIM_EnableIT_UPDATE(PWMx->TIMx);
        }
        else
        {
            LL_TIM_GenerateEvent_UPDATE(PWMx->TIMx);
        }
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
