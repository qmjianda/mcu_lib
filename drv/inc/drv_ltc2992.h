#ifndef __DRV_LTC_2992_H_
#define __DRV_LTC_2992_H_

#include "include.h"

#define LTC2992_1_DEVID_7BIT     (0x67) /*7bit地址*/
#define LTC2992_2_DEVID_7BIT     (0x6F) /*7bit地址*/

#define LTC2992_ONESHUT_MODE 0x01


#define CTRLA               (0x00) /*操作控制寄存器A*/
#define CTRLB               (0x01) /*操作控制寄存器B*/
#define ALERT1              (0x02) /* Selects Which CHANNEL 1 Faults Generate Alerts*/
#define FAULT1              (0x03) /*CHANNEL 1 Fault Log*/
#define NADC                (0x04) /*ADC分辨率*/
#define P1                  (0x05) /*POWER1 Data 0x05~0x05 3字节*/
#define MAX_P1              (0x08) /*Maximum POWER1 Data 3字节*/
#define MIN_P1              (0x0B) /*Minimum POWER1 Data 3字节*/
#define MAX_P1_THRESHOLD    (0x0E) /*Maximum POWER1 Threshold to Generate Alert 3字节*/
#define MIN_P1_THRESHOLD    (0x11) /*Minimum POWER1 Threshold to Generate Alert 3字节*/

#define I1                  (0x14) /*△SENSE1 Data 2字节 51.2mv*/
#define MAX_I1              (0x16) /*Maximum △SENSE1 Data 2字节*/
#define MIN_I1              (0x18) /*Minimum △SENSE1 Data 2字节*/
#define MAX_I1_THRESHOLD    (0x1A) /*Maximum △SENSE1 Threshold to Generate Alert 3字节*/
#define MIN_I1_THRESHOLD    (0x1C) /*Minimum △SENSE1 Threshold to Generate Alert 3字节*/

#define S1                  (0x1E) /*SENSE1+ Data 2字节 102.4V满量程*/
#define MAX_S1              (0x20) /*Maximum SENSE1+ Data 2字节*/
#define MIN_S1              (0x22) /*Minimum SENSE1+ Data 2字节*/
#define MAX_S1_THRESHOLD    (0x24) /*Maximum SENSE1+ Threshold to Generate Alert 3字节*/
#define MIN_S1_THRESHOLD    (0x26) /*Minimum SENSE1+ Threshold to Generate Alert 3字节*/

#define G1                  (0x28) /*GPIO1 Data 2字节 2.048V满量程*/
#define MAX_G1              (0x2A) /*Maximum GPIO1 Data 2字节*/
#define MIN_G1              (0x2C) /*Minimum GPIO1 Data 2字节*/
#define MAX_G1_THRESHOLD    (0x2E) /*Maximum GPIO1 Threshold to Generate Alert 3字节*/
#define MIN_G1_THRESHOLD    (0x30) /*Minimum GPIO1 Threshold to Generate Alert 3字节*/

#define ADC_STATUS          (0x32) /*ADC Status Information*/

#define ALERT2              (0x34) /* Selects Which CHANNEL 1 Faults Generate Alerts*/
#define FAULT2              (0x35) /*CHANNEL 1 Fault Log*/
#define P2                  (0x37) /*POWER1 Data 0x05~0x05 3字节*/
#define MAX_P2              (0x3A) /*Maximum POWER1 Data 3字节*/
#define MIN_P2              (0x3D) /*Minimum POWER1 Data 3字节*/
#define MAX_P2_THRESHOLD    (0x40) /*Maximum POWER1 Threshold to Generate Alert 3字节*/
#define MIN_P2_THRESHOLD    (0x43) /*Minimum POWER1 Threshold to Generate Alert 3字节*/

#define I2                  (0x46) /*△SENSE2 Data 2字节 51.2mv*/
#define MAX_I2              (0x48) /*Maximum △SENSE1 Data 2字节*/
#define MIN_I2              (0x4A) /*Minimum △SENSE1 Data 2字节*/
#define MAX_I2_THRESHOLD    (0x4C) /*Maximum △SENSE1 Threshold to Generate Alert 3字节*/
#define MIN_I2_THRESHOLD    (0x4E) /*Minimum △SENSE1 Threshold to Generate Alert 3字节*/

#define S2                  (0x50) /*SENSE2+ Data 2字节 102.4V满量程*/
#define MAX_S2              (0x52) /*Maximum SENSE1+ Data 2字节*/
#define MIN_S2              (0x54) /*Minimum SENSE1+ Data 2字节*/
#define MAX_S2_THRESHOLD    (0x56) /*Maximum SENSE1+ Threshold to Generate Alert 3字节*/
#define MIN_S2_THRESHOLD    (0x58) /*Minimum SENSE1+ Threshold to Generate Alert 3字节*/

#define G2                  (0x5A) /*GPIO2 Data 2字节 2.048V满量程*/
#define MAX_G2              (0x5C) /*Maximum GPIO2 Data 2字节*/
#define MIN_G2              (0x5E) /*Minimum GPIO2 Data 2字节*/
#define MAX_G2_THRESHOLD    (0x60) /*Maximum GPIO2 Threshold to Generate Alert 3字节*/
#define MIN_G2_THRESHOLD    (0x62) /*Minimum GPIO2 Threshold to Generate Alert 3字节*/

#define G3                  (0x64) /*GPIO3 Data 2字节 2.048V满量程*/
#define MAX_G3              (0x66) /*Maximum GPIO3 Data 2字节*/
#define MIN_G3              (0x68) /*Minimum GPIO3 Data 2字节*/
#define MAX_G3_THRESHOLD    (0x6A) /*Maximum GPIO3 Threshold to Generate Alert 3字节*/
#define MIN_G3_THRESHOLD    (0x6C) /*Minimum GPIO3 Threshold to Generate Alert 3字节*/

#define G4                  (0x6E) /*GPIO4 Data 2字节 2.048V满量程*/
#define MAX_G4              (0x70) /*Maximum GPIO4 Data 2字节*/
#define MIN_G4              (0x72) /*Minimum GPIO4 Data 2字节*/
#define MAX_G4_THRESHOLD    (0x74) /*Maximum GPIO4 Threshold to Generate Alert 3字节*/
#define MIN_G4_THRESHOLD    (0x76) /*Minimum GPIO4 Threshold to Generate Alert 3字节*/

#define ISUM                (0x78) /*(△SENSE1 + △SENSE2) Data*/
#define MAX_ISUM            (0x7A) /*Maximum(△SENSE1 + △SENSE2) Data*/
#define MIN_ISUM            (0x7C) /*Minimum(△SENSE1 + △SENSE2) Data*/
#define MAX_ISUM_THRESHOLD  (0x7E) /*Maximum (△SENSE1 + △SENSE2) Threshold to Generate Alert 3字节*/
#define MIN_ISUM_THRESHOLD  (0x80) /*Minimum (△SENSE1 + △SENSE2) Threshold to Generate Alert 3字节*/

#define PSUM                (0x82) /*(POWER1 + POWER2) Data 3*/
#define MAX_PSUM            (0x85) /*Maximum (POWER1 + POWER2) Data 3*/
#define MIN_PSUM            (0x88) /*Minimum (POWER1 + POWER2) Data 3*/
#define MAX_PSUM_THRESHOLD  (0x8B) /*Maximum (POWER1 + POWER2) Threshold to Generate Alert 3字节*/
#define MIN_PSUM_THRESHOLD  (0x8E) /*Minimum (POWER1 + POWER2) Threshold to Generate Alert 3字节*/

#define ALERT3              (0x91) /*Selects Which GPIO or Total Current/Power Faults Generate Alerts*/
#define FAULT3              (0x92) /*GPIO and Total Current/Power Fault Log*/
#define ALERT4              (0x93) /*Selects Which Additional Faults Generate Alerts */
#define FAULT4              (0x94) /*Additional Fault Log*/

#define GPIO_STATUS         (0x95) /*GPIO Status Information*/
#define GPIO_CONTROL        (0x96) /*GPIO1,2,3 Input/Output Control Command*/
#define GPIO4_CONTROL       (0x97) /*GPIO4 Control Command*/

#define MFR_SPECIAL_ID_MSB  (0xE7) /*Manufacturer Special ID MSB Data*/
#define MFR_SPECIAL_ID_LSB  (0xE8) /*Manufacturer Special ID LSB Data*/

#define PWIN_MON_DATAREADY_PORT                 (GPIOB)
#define PWIN_MON_DATAREADY_PIN                  (GPIO_Pin_5)
#define PWIN_MON_DATAREADY_IRQn                 (EXTI9_5_IRQn)
#define PWIN_MON_DATAREADY_EXTI_LINE            (EXTI_Line5)
#define PWIN_MON_DATAREADY_EXTI_PORT_SOURCE     (GPIO_PortSourceGPIOB)
#define PWIN_MON_DATAREADY_EXTI_PIN_SOURCE      (GPIO_PinSource5)

#define PWOUT_MON_DATAREADY_PORT                (GPIOB)
#define PWOUT_MON_DATAREADY_PIN                 (GPIO_Pin_12)
#define PWOUT_MON_DATAREADY_IRQn                (EXTI15_10_IRQn)
#define PWOUT_MON_DATAREADY_EXTI_LINE           (EXTI_Line12)
#define PWOUT_MON_DATAREADY_EXTI_PORT_SOURCE    (GPIO_PortSourceGPIOB)
#define PWOUT_MON_DATAREADY_EXTI_PIN_SOURCE     (GPIO_PinSource12)

/***********************底层******************************************/
uint8_t ltc2992WrByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t data);
uint8_t ltc2992WrNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* data, uint8_t Len);
uint8_t ltc2992RdByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata);
uint8_t ltc2992RdNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata, uint8_t Len);
void ltc2992Init(void);
uint16_t ltc2992GetRaw(uint8_t ch, uint8_t regist);

void ltc2992SysPowerVoltage(float *sys24V, float *sys12V, float *sys5V, float *sys3V3); /*单位V*/
void ltc2992MotorI(float *motor_I); /*单位ma*/
void ltc2992ServoI(float *servo_I); /*单位ma*/
void ltc2992LinearDrvI(uint8_t ch, float *linearDrv_I); /*单位ma*/
/***********************底层 end**************************************/


#endif /*__LTC_2992_H_*/
