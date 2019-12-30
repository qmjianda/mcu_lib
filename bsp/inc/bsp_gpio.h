#ifndef __BSP_GPIO_H_
#define __BSP_GPIO_H_

#include "include.h"

/// GPIO Mode
typedef enum _GPIO_MODE 
{
    GPIO_MODE_INPUT     = 0x00,              /// GPIO is input
    GPIO_MODE_OUT10MHZ  = 0x01,              /// Max output Speed 10MHz
    GPIO_MODE_OUT2MHZ   = 0x02,              /// Max output Speed  2MHz
    GPIO_MODE_OUT50MHZ  = 0x03               /// Max output Speed 50MHz
} GPIO_MODE;

/// GPIO Conf
typedef enum 
{
    GPIO_OUT_PUSH_PULL  = 0x00,               /// general purpose output push-pull
    GPIO_OUT_OPENDRAIN  = 0x01,               /// general purpose output open-drain
    GPIO_AF_PUSHPULL    = 0x02,               /// alternate function push-pull
    GPIO_AF_OPENDRAIN   = 0x03,               /// alternate function open-drain
    GPIO_IN_ANALOG      = 0x00,               /// input analog
    GPIO_IN_FLOATING    = 0x01,               /// input floating
    GPIO_IN_PULL_DOWN   = 0x02,               /// alternate function push-pull
    GPIO_IN_PULL_UP     = 0x03                /// alternate function pull up
} GPIO_CONF;

#define AFIO_FUNC_DEF(bit, mask, val, reg) ((bit) | (mask << 5) | (val << 8) | (reg << 12))


/// Alternate function I/O remap
typedef enum {
  /* AF remap and debug I/O config register */
  AFIO_SPI1_NO_REMAP         = AFIO_FUNC_DEF (0,  1, 0, 0),
  AFIO_SPI1_REMAP            = AFIO_FUNC_DEF (0,  1, 1, 0),
  AFIO_I2C1_NO_REMAP         = AFIO_FUNC_DEF (1,  1, 0, 0),
  AFIO_I2C1_REMAP            = AFIO_FUNC_DEF (1,  1, 1, 0),
  AFIO_USART1_NO_REMAP       = AFIO_FUNC_DEF (2,  1, 0, 0),
  AFIO_USART1_REMAP          = AFIO_FUNC_DEF (2,  1, 1, 0),
  AFIO_USART2_NO_REMAP       = AFIO_FUNC_DEF (3,  1, 0, 0),
  AFIO_USART2_REMAP          = AFIO_FUNC_DEF (3,  1, 1, 0),
  AFIO_USART3_NO_REMAP       = AFIO_FUNC_DEF (4,  3, 0, 0),
  AFIO_USART3_REMAP_PARTIAL  = AFIO_FUNC_DEF (4,  3, 1, 0),
  AFIO_USART3_REMAP_FULL     = AFIO_FUNC_DEF (4,  3, 3, 0),
  AFIO_TIM1_NO_REMAP         = AFIO_FUNC_DEF (6,  3, 0, 0),
  AFIO_TIM1_REMAP_PARTIAL    = AFIO_FUNC_DEF (6,  3, 1, 0),
  AFIO_TIM1_REMAP_FULL       = AFIO_FUNC_DEF (6,  3, 3, 0),
  AFIO_TIM2_NO_REMAP         = AFIO_FUNC_DEF (8,  3, 0, 0),
  AFIO_TIM2_REMAP_PARTIAL_1  = AFIO_FUNC_DEF (8,  3, 1, 0),
  AFIO_TIM2_REMAP_PARTIAL_2  = AFIO_FUNC_DEF (8,  3, 2, 0),
  AFIO_TIM2_REMAP_FULL       = AFIO_FUNC_DEF (8,  3, 3, 0),
  AFIO_TIM3_NO_REMAP         = AFIO_FUNC_DEF (10, 3, 0, 0),
  AFIO_TIM3_REMAP_PARTIAL    = AFIO_FUNC_DEF (10, 3, 2, 0),
  AFIO_TIM3_REMAP_FULL       = AFIO_FUNC_DEF (10, 3, 3, 0),
  AFIO_TIM4_NO_REMAP         = AFIO_FUNC_DEF (12, 1, 0, 0),
  AFIO_TIM4_REMAP            = AFIO_FUNC_DEF (12, 1, 1, 0),
  AFIO_CAN_PA11_PA12         = AFIO_FUNC_DEF (13, 3, 0, 0),
  AFIO_CAN_PB8_PB9           = AFIO_FUNC_DEF (13, 3, 2, 0),
  AFIO_CAN_PD0_PD1           = AFIO_FUNC_DEF (13, 3, 3, 0),
  AFIO_PD01_NO_REMAP         = AFIO_FUNC_DEF (15, 1, 0, 0),
  AFIO_PD01_REMAP            = AFIO_FUNC_DEF (15, 1, 1, 0),
  AFIO_TIM5CH4_NO_REMAP      = AFIO_FUNC_DEF (16, 1, 0, 0),
  AFIO_TIM5CH4_REMAP         = AFIO_FUNC_DEF (16, 1, 1, 0),
  AFIO_ADC1_ETRGINJ_NO_REMAP = AFIO_FUNC_DEF (17, 1, 0, 0),
  AFIO_ADC1_ETRGINJ_REMAP    = AFIO_FUNC_DEF (17, 1, 1, 0),
  AFIO_ADC1_ETRGREG_NO_REMAP = AFIO_FUNC_DEF (18, 1, 0, 0),
  AFIO_ADC1_ETRGREG_REMAP    = AFIO_FUNC_DEF (18, 1, 1, 0),
  AFIO_ADC2_ETRGINJ_NO_REMAP = AFIO_FUNC_DEF (19, 1, 0, 0),
  AFIO_ADC2_ETRGINJ_REMAP    = AFIO_FUNC_DEF (19, 1, 1, 0),
  AFIO_ADC2_ETRGREG_NO_REMAP = AFIO_FUNC_DEF (20, 1, 0, 0),
  AFIO_ADC2_ETRGREG_REMAP    = AFIO_FUNC_DEF (20, 1, 1, 0),
#if defined(STM32F10X_CL)
  AFIO_ETH_NO_REMAP          = AFIO_FUNC_DEF (21, 1, 0, 0),
  AFIO_ETH_REMAP             = AFIO_FUNC_DEF (21, 1, 1, 0),
  AFIO_CAN2_NO_REMAP         = AFIO_FUNC_DEF (22, 1, 0, 0),
  AFIO_CAN2_REMAP            = AFIO_FUNC_DEF (22, 1, 1, 0),
  AFIO_ETH_MII_SEL           = AFIO_FUNC_DEF (23, 1, 0, 0),
  AFIO_ETH_RMII_SEL          = AFIO_FUNC_DEF (23, 1, 1, 0),
#endif
  AFIO_SWJ_FULL              = AFIO_FUNC_DEF (24, 7, 0, 0),
  AFIO_SWJ_FULL_NO_NJTRST    = AFIO_FUNC_DEF (24, 7, 1, 0),
  AFIO_SWJ_JTAG_NO_SW        = AFIO_FUNC_DEF (24, 7, 2, 0),
  AFIO_SWJ_NO_JTAG_NO_SW     = AFIO_FUNC_DEF (24, 7, 4, 0),
#if defined(STM32F10X_CL)
  AFIO_SPI3_NO_REMAP         = AFIO_FUNC_DEF (28, 1, 0, 0),
  AFIO_SPI3_REMAP            = AFIO_FUNC_DEF (28, 1, 1, 0),
  AFIO_TIM2ITR_NO_REMAP      = AFIO_FUNC_DEF (29, 1, 0, 0),
  AFIO_TIM2ITR_REMAP         = AFIO_FUNC_DEF (29, 1, 1, 0),
  AFIO_PTP_PPS_NO_REMAP      = AFIO_FUNC_DEF (30, 1, 0, 0),
  AFIO_PTP_PPS_REMAP         = AFIO_FUNC_DEF (30, 1, 1, 0),
#endif
  
  /* AF remap and debug I/O config register 2 */
  AFIO_TIM15_NO_REMAP        = AFIO_FUNC_DEF (0,  1, 0, 1),
  AFIO_TIM15_REMAP           = AFIO_FUNC_DEF (0,  1, 1, 1),
  AFIO_TIM16_NO_REMAP        = AFIO_FUNC_DEF (1,  1, 0, 1),
  AFIO_TIM16_REMAP           = AFIO_FUNC_DEF (1,  1, 1, 1),
  AFIO_TIM17_NO_REMAP        = AFIO_FUNC_DEF (2,  1, 0, 1),
  AFIO_TIM17_REMAP           = AFIO_FUNC_DEF (2,  1, 1, 1),
  AFIO_CEC_NO_REMAP          = AFIO_FUNC_DEF (3,  1, 0, 1),
  AFIO_CEC_REMAP             = AFIO_FUNC_DEF (3,  1, 1, 1),
  AFIO_TIM1_DMA_NO_REMAP     = AFIO_FUNC_DEF (4,  1, 0, 1),
  AFIO_TIM1_DMA_REMAP        = AFIO_FUNC_DEF (4,  1, 1, 1),
  
  AFIO_TIM9_NO_REMAP         = AFIO_FUNC_DEF (5,  1, 0, 1),
  AFIO_TIM9_REMAP            = AFIO_FUNC_DEF (5,  1, 1, 1),
  AFIO_TIM10_NO_REMAP        = AFIO_FUNC_DEF (6,  1, 0, 1),
  AFIO_TIM10_REMAP           = AFIO_FUNC_DEF (6,  1, 1, 1),
  AFIO_TIM11_NO_REMAP        = AFIO_FUNC_DEF (7,  1, 0, 1),
  AFIO_TIM11_REMAP           = AFIO_FUNC_DEF (7,  1, 1, 1),
  AFIO_TIM13_NO_REMAP        = AFIO_FUNC_DEF (8,  1, 0, 1),
  AFIO_TIM13_REMAP           = AFIO_FUNC_DEF (8,  1, 0, 1),
  AFIO_TIM14_NO_REMAP        = AFIO_FUNC_DEF (9,  1, 0, 1),
  AFIO_TIM14_REMAP           = AFIO_FUNC_DEF (9,  1, 1, 1),
  AFIO_FSMC_NADV_NO_REMAP    = AFIO_FUNC_DEF (10, 1, 0, 1),
  AFIO_FSMC_NADV_REMAP       = AFIO_FUNC_DEF (10, 1, 1, 1),

  AFIO_TIM67_DAC_DMA_NO_REMAP = AFIO_FUNC_DEF(11, 1, 0, 1),
  AFIO_TIM67_DAC_DMA_REMAP   = AFIO_FUNC_DEF (11, 1, 1, 1),
  AFIO_TIM12_NO_REMAP        = AFIO_FUNC_DEF (12, 1, 0, 1),
  AFIO_TIM12_REMAP           = AFIO_FUNC_DEF (12, 1, 1, 1),
  AFIO_MISC_NO_REMAP         = AFIO_FUNC_DEF (13, 1, 0, 1),
  AFIO_MISC_REMAP            = AFIO_FUNC_DEF (13, 1, 1, 1),

  /* Reserved value */
  AFIO_UNAVAILABLE_REMAP     = AFIO_FUNC_DEF (0,  0, 0, 0)
} AFIO_REMAP;

typedef struct
{
    GPIO_TypeDef *GPIOx;
    uint32_t      num;
}T_GpioPin;

void gpioClkEnable(GPIO_TypeDef *GPIOx, bool enable);

bool gpioConfig(GPIO_TypeDef      *GPIOx,
               uint32_t           num,
               GPIO_CONF          conf,
               GPIO_MODE          mode);

void gpioPinAF (AFIO_REMAP af_type);

static inline void gpioPinWrite(GPIO_TypeDef *GPIOx, uint32_t num, uint32_t val) 
{
    if (val & 1) 
    {
        GPIOx->BSRR = (1UL << num);
    } 
    else 
    {
        GPIOx->BSRR = (1UL << (num + 16));
    }
}

static inline uint32_t gpioPinRead (GPIO_TypeDef *GPIOx, uint32_t num)
{
    return ((GPIOx->IDR >> num) & 1);
}

static inline void gpioPortWrite(GPIO_TypeDef *GPIOx, uint16_t mask, uint16_t val) 
{
    GPIOx->ODR = (GPIOx->ODR & ~mask) | val;
}

static inline uint16_t gpioPortRead(GPIO_TypeDef *GPIOx) 
{
    return (GPIOx->IDR);
}

#endif /*__BSP_GPIO_H_*/
