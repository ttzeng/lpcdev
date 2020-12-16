/*
 * lpc8xx_swm.h
 *
 *  Created on: Feb 16, 2016
 *
 */

#ifndef LPC8XX_SWM_H_
#define LPC8XX_SWM_H_

// Port pin number equates
#define P0_0      0
#define P0_1      1
#define P0_2      2
#define P0_3      3
#define P0_4      4
#define P0_5      5
#define P0_6      6
#define P0_7      7
#define P0_8      8
#define P0_9      9
#define P0_10    10
#define P0_11    11
#define P0_12    12
#define P0_13    13
#define P0_14    14
#define P0_15    15
#define P0_16    16
#define P0_17    17
#define P0_18    18
#define P0_19    19
#define P0_20    20
#define P0_21    21
#define P0_22    22
#define P0_23    23
#define P0_24    24
#define P0_25    25
#define P0_26    26
#define P0_27    27
#define P0_28    28

// Function name equates (exact match with user manual)
#define U0_TXD_O      0
#define U0_RXD_I      1
#define U0_RTS_O      2
#define U0_CTS_I      3
#define U0_SCLK_IO    4
#define U1_TXD_O      5
#define U1_RXD_I      6
#define U1_RTS_O      7
#define U1_CTS_I      8
#define U1_SCLK_IO    9
#define U2_TXD_O      10
#define U2_RXD_I      11
#define U2_RTS_O      12
#define U2_CTS_I      13
#define U2_SCLK_IO    14
#define SPI0_SCK_IO   15
#define SPI0_MOSI_IO  16
#define SPI0_MISO_IO  17
#define SPI0_SSEL_IO  18
#define SPI1_SCK_IO   19
#define SPI1_MOSI_IO  20
#define SPI1_MISO_IO  21
#define SPI1_SSEL_IO  22
#define CTIN_0_I      23
#define CTIN_1_I      24
#define CTIN_2_I      25
#define CTIN_3_I      26
#define CTOUT_0_O     27
#define CTOUT_1_O     28
#define CTOUT_2_O     29
#define CTOUT_3_O     30
#define I2C0_SDA_IO   31
#define I2C0_SCL_IO   32
#define ACMP_O_O      33
#define CLKOUT_O      34
#define GPIO_INT_BMAT_O 35

// Function name equates (for normal people)
#define U0_TXD        0
#define U0_RXD        1
#define U0_RTS        2
#define U0_CTS        3
#define U0_SCLK       4
#define U1_TXD        5
#define U1_RXD        6
#define U1_RTS        7
#define U1_CTS        8
#define U1_SCLK       9
#define U2_TXD        10
#define U2_RXD        11
#define U2_RTS        12
#define U2_CTS        13
#define U2_SCLK       14
#define SPI0_SCK      15
#define SPI0_MOSI     16
#define SPI0_MISO     17
#define SPI0_SSEL     18
#define SPI1_SCK      19
#define SPI1_MOSI     20
#define SPI1_MISO     21
#define SPI1_SSEL     22
#define CTIN_0        23
#define CTIN_1        24
#define CTIN_2        25
#define CTIN_3        26
#define CTOUT_0       27
#define CTOUT_1       28
#define CTOUT_2       29
#define CTOUT_3       30
#define I2C0_SDA      31
#define I2C0_SCL      32
#define ACOMP         33
#define CLKOUT        34
#define GPIO_INT_BMAT 35

#define num_funcs 36



// PINENABLE0 defines
#define ACMP_I1   (1<<0)
#define ACMP_I2   (1<<1)
#define SWCLK     (1<<2)
#define SWDIO     (1<<3)
#define XTALIN    (1<<4)
#define XTALOUT   (1<<5)
#define RESETN    (1<<6)
#define CLKIN     (1<<7)
#define VDDCMP    (1<<8)
//#define I2C0_SDA  (1<<11)
//#define I2C0_SCL  (1<<12)
//#define ADC_0     (1<<13)
//#define ADC_1     (1<<14)
//#define ADC_2     (1<<15)
//#define ADC_3     (1<<16)
//#define ADC_4     (1<<17)
//#define ADC_5     (1<<18)
//#define ADC_6     (1<<19)
//#define ADC_7     (1<<20)
//#define ADC_8     (1<<21)
//#define ADC_9     (1<<22)
//#define ADC_10    (1<<23)
//#define ADC_11    (1<<24)





#endif /* LPC8XX_SWM_H_ */
