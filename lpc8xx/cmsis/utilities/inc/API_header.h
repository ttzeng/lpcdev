/*
 * API_header.h
 *
 *  Created on: Apr 21, 2016
 *      Arthur:
 */

#ifndef API_HEADER_H_
#define API_HEADER_H_


/////////////////////////////////////// Power APIs ////////////////////////////////////////
typedef struct _PWRD {
  void (*set_pll)(unsigned int cmd[], unsigned int resp[]);
  void (*set_power)(unsigned int cmd[], unsigned int resp[]);
} PWRD_API_T;
/////////////////////////////////////// End of Power APIs /////////////////////////////////







////////////////////////////////////////////// UART APIs ////////////////////////////////////////
typedef void*  UART_HANDLE_T ;                                                        // define TYPE for uart handle pointer
typedef void  (*UART_CALLBK_T) (uint32_t   err_code, uint32_t n ) ;                   // define callback func TYPE
typedef void  (*UART_DMA_REQ_T) (uint32_t src_adr, uint32_t dst_adr, uint32_t size) ; // define Uart DMA channel setup func TYPE

typedef struct{
  uint32_t sys_clk_in_hz;
  uint32_t baudrate_in_hz;
  uint8_t  config;  //bit1:0
                    //   00: 7 bits length, 01: 8 bits lenght, others: reserved
                    //bit3:2
                    //   00: No Parity, 01: reserved, 10: Even, 11: Odd
                    //bit4
                    //   0: 1 Stop bit, 1: 2 Stop bits
  uint8_t sync_mod; //bit0: 0(Async mode), 1(Sync mode)
                    //bit1: 0(Un_RXD is sampled on the falling edge of SCLK)
                    //      1(Un_RXD is sampled on the rising edge of SCLK)
                    //bit2: 0(Start and stop bits are transmitted as in asynchronous mode)
                    //      1(Start and stop bits are not transmitted)
                    //bit3: 0(the UART is a slave on Sync mode)
                    //      1(the UART is a master on Sync mode)
  uint16_t error_en; //Bit0: OverrunEn, bit1: UnderrunEn, bit2: FrameErrEn, bit3: ParityErrEn, bit4: RxNoiseEn
} UART_CONFIG_T;

typedef struct {		 // params passed to uart driver function
  uint8_t        *buffer ;
  uint32_t       size;
  uint16_t       transfer_mode; // 0x00: For uart_get_line function, stop transfer when the buffer is full.
                                //       For uart_put_line function, stop transfer when the buffer is empty.
                                // 0x01: For uart_get_line function, stop transfer when <CR><LF> are received.
                                //       For uart_put_line function, transfer is stopped after reaching \0. And <CR><LF> is sent out after that.
                                // 0x02: For uart_get_line function, stop transfer when <LF> are received.
                                //       For uart_put_line function, transfer is stopped after reaching \0. And <LF> is sent out after that.
                                // 0x03: For uart_get_line function, RESERVED.
                                //       For uart_put_line function, transfer is stopped after reaching \0.
  uint16_t       driver_mode;   // 0x00: Polling mode, function is blocked until transfer is finished.
                                // 0x01: Intr mode, function exit immediately, callback function is invoked when transfer is finished.
                                // 0x02: DMA mode, in case DMA block is available, DMA req function is called for Uart DMA channel setup, then callback function indicate that transfer is finished.
  UART_CALLBK_T  callback_func_pt; // callback function
  UART_DMA_REQ_T dma_req_func_pt;  // Uart DMA channel setup function
} UART_PARAM_T;


//************************************************
// *** Below is the structure providing the function types for all the
//     UART functions.  One structure is used to pass params to most of the functions.
//     The parms that are required to be set before calling the function are
//     listed in comments below each function.	***

typedef struct  UARTD_API {	   // index of all the uart driver functions
  uint32_t (*uart_get_mem_size)(void);
  UART_HANDLE_T (*uart_setup)(uint32_t base_addr, uint8_t *ram);
  uint32_t (*uart_init)(UART_HANDLE_T handle, UART_CONFIG_T *set);
  //--polling functions--//
  uint8_t (*uart_get_char)(UART_HANDLE_T handle);
  void (*uart_put_char)(UART_HANDLE_T handle, uint8_t data);
  uint32_t (*uart_get_line)(UART_HANDLE_T handle, UART_PARAM_T * param);
  uint32_t (*uart_put_line)(UART_HANDLE_T handle, UART_PARAM_T * param);
  //--interrupt functions--//
  void (*uart_isr)(UART_HANDLE_T handle);
} UARTD_API_T  ;	// end of structure ************************************

extern const UARTD_API_T  uart_api ; //so application program can access pointer to function table
/////////////////////////////////// End of UART APIs ////////////////////////////////////////////








/////////////////////////////////////// I2C APIs ///////////////////////////////////////////////
typedef void* I2C_HANDLE_T;
typedef void (*I2C_CALLBK_T) (uint32_t err_code, uint32_t n);


typedef struct I2C_PARAM {  // PARAM struct--parameters passed to ROM function
  uint32_t num_bytes_send;
  uint32_t num_bytes_rec;
  uint8_t *buffer_ptr_send;
  uint8_t *buffer_ptr_rec;
  I2C_CALLBK_T func_pt; // callback function pointer
  uint8_t stop_flag;
  uint8_t dummy[3];     // required for word alignment
} I2C_PARAM_T;


typedef struct I2C_RESULT { // RESULTs struct--results are here when returned
uint32_t n_bytes_sent ;
uint32_t n_bytes_recd ;
} I2C_RESULT_T;


typedef enum
{
  LPC_OK = 0,                           // enum value returned on Success
  ERROR,
  ERR_I2C_BASE = 0x00060000,
  ERR_I2C_NAK = ERR_I2C_BASE+1,         // 0x00060001
  ERR_I2C_BUFFER_OVERFLOW,              // 0x00060002
  ERR_I2C_BYTE_COUNT_ERR,               // 0x00060003
  ERR_I2C_LOSS_OF_ARBRITRATION,         // 0x00060004
  ERR_I2C_SLAVE_NOT_ADDRESSED,          // 0x00060005
  ERR_I2C_LOSS_OF_ARBRITRATION_NAK_BIT, // 0x00060006
  ERR_I2C_GENERAL_FAILURE,              // 0x00060007
  ERR_I2C_REGS_SET_TO_DEFAULT           // 0x00060008
} ErrorCode_t;


typedef enum CHIP_I2C_mode {
  IDLE,
  MASTER_SEND,
  MASTER_RECEIVE,
  SLAVE_SEND,
  SLAVE_RECEIVE
} CHIP_I2C_MODE_T;


typedef struct I2CD_API { // index of all the i2c driver functions
  // ISR
  void (*i2c_isr_handler) (I2C_HANDLE_T* h_i2c);
  // MASTER functions ***
  ErrorCode_t (*i2c_master_transmit_poll)(I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_master_receive_poll) (I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_master_tx_rx_poll)   (I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_master_transmit_intr)(I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_master_receive_intr) (I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_master_tx_rx_intr)   (I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  // SLAVE functions ***
  ErrorCode_t (*i2c_slave_receive_poll) (I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_slave_transmit_poll)(I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_slave_receive_intr) (I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_slave_transmit_intr)(I2C_HANDLE_T* h_i2c, I2C_PARAM_T* ptp, I2C_RESULT_T* ptr);
  ErrorCode_t (*i2c_set_slave_addr)     (I2C_HANDLE_T* h_i2c, uint32_t slave_addr_0_3, uint32_t slave_mask_0_3);
  // OTHER functions
  uint32_t      (*i2c_get_mem_size)(void); //ramsize_in_bytes memory needed by I2C drivers
  I2C_HANDLE_T* (*i2c_setup)       (uint32_t i2c_base_addr, uint32_t *start_of_ram );
  ErrorCode_t   (*i2c_set_bitrate) (I2C_HANDLE_T* h_i2c, uint32_t P_clk_in_hz, uint32_t bitrate_in_bps);
  uint32_t      (*i2c_get_firmware_version)();
  CHIP_I2C_MODE_T (*i2c_get_status)  (I2C_HANDLE_T* h_i2c );
} I2CD_API_T;


typedef struct _ROM_API {
  const uint32_t unused[3];
  const PWRD_API_T *pPWRD;
//const uint32_t p_dev4;    // Placeholder for const PWRD_API_T *pPWRD;
  const uint32_t p_dev5;
  const I2CD_API_T *pI2CD;
  const uint32_t p_dev7;
  const uint32_t p_dev8;
  const uint32_t p_dev9;
  const UARTD_API_T *pUARTD;
} ROM_API_T;
/////////////////////////////////// End of I2C APIs ////////////////////////////////////////////


#define ROM_DRIVER_BASE (0x1FFF1FF8UL)
#define LPC_PWRD_API ((PWRD_API_T  *) ((*(ROM_API_T * *) (ROM_DRIVER_BASE))->pPWRD))
#define LPC_UART_API ((UARTD_API_T *) ((*(ROM_API_T * *) (ROM_DRIVER_BASE))->pUARTD))
#define LPC_I2CD_API ((I2CD_API_T  *) ((*(ROM_API_T * *) (ROM_DRIVER_BASE))->pI2CD))



#endif /* API_HEADER_H_ */
