/*
 * max30003.h
 *
 *  Created on: 2019Äê5ÔÂ14ÈÕ
 *      Author: D404-ky
 */

#ifndef __MAIN_IC_DRIVER_MAX30003_H_
#define __MAIN_IC_DRIVER_MAX30003_H_

#include <stdint.h>


const uint8_t _ECG3_NO_OP_REG                     = 0x00;
const uint8_t _ECG3_STAT_REG                      = 0x01;
const uint8_t _ECG3_EN_INT_REG                    = 0x02;
const uint8_t _ECG3_EN_INT2_REG                   = 0x03;
const uint8_t _ECG3_MNGR_INT_REG                  = 0x04;
const uint8_t _ECG3_MNGR_DYN_REG                  = 0x05;
const uint8_t _ECG3_SW_RST_REG                    = 0x08;
const uint8_t _ECG3_SYNC_REG                      = 0x09;
const uint8_t _ECG3_FIFO_RST_REG                  = 0x0A;
const uint8_t _ECG3_INFO_REG                      = 0x0F;
const uint8_t _ECG3_CNFG_GEN_REG                  = 0x10;
const uint8_t _ECG3_CNFG_CAL_REG                  = 0x12;
const uint8_t _ECG3_CNFG_EMUX_REG                 = 0x14;
const uint8_t _ECG3_CNFG_ECG_REG                  = 0x15;
const uint8_t _ECG3_CNFG_RTOR1_REG                = 0x1D;
const uint8_t _ECG3_CNFG_RTOR2_REG                = 0x1E;
const uint8_t _ECG3_ECG_FIFO_BURST_REG            = 0x20;
const uint8_t _ECG3_ECG_FIFO_REG                  = 0x21;
const uint8_t _ECG3_RTOR_REG                      = 0x25;
const uint8_t _ECG3_NO_OP2_REG                    = 0x7F;


const uint32_t _ECG3_SW_RST_CMD                   = 0x000000;
const uint32_t _ECG3_FIFO_RST_CMD                 = 0x000000;
const uint32_t _ECG3_SYNCH_CMD                    = 0x000000;

const uint32_t _ECG3_ULP_LON_EN                   = 0x400000;
const uint32_t _ECG3_FMSTR_32768HZ_ECG_512HZ      = 0x000000;
const uint32_t _ECG3_FMSTR_32000HZ_ECG_500HZ      = 0x100000;
const uint32_t _ECG3_FMSTR_32000HZ_ECG_200HZ      = 0x200000;
const uint32_t _ECG3_FMSTR_31968HZ_ECG_199HZ      = 0x300000;
const uint32_t _ECG3_ECG_CHANN_EN                 = 0x080000;
const uint32_t _ECG3_DCLOFF_EN                    = 0x001000;
const uint32_t _ECG3_ECGP_PULLUP                  = 0x000000;
const uint32_t _ECG3_ECGP_PULLDOWN                = 0x000800;
const uint32_t _ECG3_DCLOFF_IMAG_0NA              = 0x000000;
const uint32_t _ECG3_DCLOFF_IMAG_5NA              = 0x000100;
const uint32_t _ECG3_DCLOFF_IMAG_10NA             = 0x000200;
const uint32_t _ECG3_DCLOFF_IMAG_20NA             = 0x000300;
const uint32_t _ECG3_DCLOFF_IMAG_50NA             = 0x000400;
const uint32_t _ECG3_DCLOFF_IMAG_100NA            = 0x000500;
const uint32_t _ECG3_DCLOFF_VTH_300MV             = 0x000000;
const uint32_t _ECG3_DCLOFF_VTH_400MV             = 0x000040;
const uint32_t _ECG3_DCLOFF_VTH_450MV             = 0x000080;
const uint32_t _ECG3_DCLOFF_VTH_500MV             = 0x0000C0;
const uint32_t _ECG3_RBIAS_EN                     = 0x000010;
const uint32_t _ECG3_RBIAS_50M_OHM                = 0x000000;
const uint32_t _ECG3_RBIAS_100M_OHM               = 0x000004;
const uint32_t _ECG3_RBIAS_200M_OHM               = 0x000008;
const uint32_t _ECG3_RBIASP_EN                    = 0x000002;
const uint32_t _ECG3_RBIASN_EN                    = 0x000001;

const uint32_t _ECG3_VCAL_EN                      = 0x400000;
const uint32_t _ECG3_VMODE_UNIPOL                 = 0x000000;
const uint32_t _ECG3_VMODE_BIPOL                  = 0x200000;
const uint32_t _ECG3_VMAG_250MICROV               = 0x000000;
const uint32_t _ECG3_VMAG_500MICROV               = 0x100000;
const uint32_t _ECG3_FCAL_256HZ                   = 0x000000;
const uint32_t _ECG3_FCAL_64HZ                    = 0x001000;
const uint32_t _ECG3_FCAL_16HZ                    = 0x002000;
const uint32_t _ECG3_FCAL_4HZ                     = 0x003000;
const uint32_t _ECG3_FCAL_1HZ                     = 0x004000;
const uint32_t _ECG3_FCAL_1PER4HZ                 = 0x005000;
const uint32_t _ECG3_FCAL_1PER16HZ                = 0x006000;
const uint32_t _ECG3_FCAL_1PER64HZ                = 0x007000;
const uint32_t _ECG3_FIFTY_CAL_THIGH              = 0x000000;
const uint32_t _ECG3_FIFTY_50PERCENTS             = 0x000800;

const uint32_t _ECG3_INPUT_NON_INV                = 0x000000;
const uint32_t _ECG3_INPUT_INV                    = 0x800000;
const uint32_t _ECG3_ECGP_EN                      = 0x000000;
const uint32_t _ECG3_ECGP_DIS                     = 0x200000;
const uint32_t _ECG3_ECGN_EN                      = 0x000000;
const uint32_t _ECG3_ECGN_DIS                     = 0x100000;
const uint32_t _ECG3_ECGP_NO_CAL                  = 0x000000;
const uint32_t _ECG3_ECGP_CAL_VMID                = 0x040000;
const uint32_t _ECG3_ECGP_CAL_VCALP               = 0x080000;
const uint32_t _ECG3_ECGP_CAL_VCALN               = 0x0C0000;
const uint32_t _ECG3_ECGN_NO_CAL                  = 0x000000;
const uint32_t _ECG3_ECGN_CAL_VMID                = 0x010000;
const uint32_t _ECG3_ECGN_CAL_VCALP               = 0x020000;
const uint32_t _ECG3_ECGN_CAL_VCALN               = 0x030000;

const uint32_t _ECG3_GAIN_20VPERV                 = 0x000000;
const uint32_t _ECG3_GAIN_40VPERV                 = 0x010000;
const uint32_t _ECG3_GAIN_80VPERV                 = 0x020000;
const uint32_t _ECG3_GAIN_160VPERV                = 0x030000;
const uint32_t _ECG3_DHPF_BYPASS_DC               = 0x000000;
const uint32_t _ECG3_DHPF_500MILIHZ               = 0x004000;
const uint32_t _ECG3_DLPF_BYPASS                  = 0x000000;
const uint32_t _ECG3_DLPF_40HZ                    = 0x001000;
const uint32_t _ECG3_DLPF_100HZ                   = 0x002000;
const uint32_t _ECG3_DLPF_150HZ                   = 0x003000;

const uint32_t _ECG3_WNDW_6                       = 0x000000;
const uint32_t _ECG3_WNDW_8                       = 0x100000;
const uint32_t _ECG3_WNDW_10                      = 0x200000;
const uint32_t _ECG3_WNDW_12                      = 0x300000;
const uint32_t _ECG3_WNDW_14                      = 0x400000;
const uint32_t _ECG3_WNDW_16                      = 0x500000;
const uint32_t _ECG3_WNDW_18                      = 0x600000;
const uint32_t _ECG3_WNDW_20                      = 0x700000;
const uint32_t _ECG3_WNDW_22                      = 0x800000;
const uint32_t _ECG3_WNDW_24                      = 0x900000;
const uint32_t _ECG3_WNDW_26                      = 0xA00000;
const uint32_t _ECG3_WNDW_28                      = 0xB00000;
const uint32_t _ECG3_RRGAIN_1                     = 0x000000;
const uint32_t _ECG3_RRGAIN_2                     = 0x010000;
const uint32_t _ECG3_RRGAIN_4                     = 0x020000;
const uint32_t _ECG3_RRGAIN_8                     = 0x030000;
const uint32_t _ECG3_RRGAIN_16                    = 0x040000;
const uint32_t _ECG3_RRGAIN_32                    = 0x050000;
const uint32_t _ECG3_RRGAIN_64                    = 0x060000;
const uint32_t _ECG3_RRGAIN_128                   = 0x070000;
const uint32_t _ECG3_RRGAIN_256                   = 0x080000;
const uint32_t _ECG3_RRGAIN_512                   = 0x090000;
const uint32_t _ECG3_RRGAIN_1024                  = 0x0A0000;
const uint32_t _ECG3_RRGAIN_2048                  = 0x0B0000;
const uint32_t _ECG3_RRGAIN_4096                  = 0x0C0000;
const uint32_t _ECG3_RRGAIN_8192                  = 0x0D0000;
const uint32_t _ECG3_RRGAIN_16384                 = 0x0E0000;
const uint32_t _ECG3_RRGAIN_AUTO_SCALE            = 0x0F0000;
const uint32_t _ECG3_RTOR_EN                      = 0x008000;
const uint32_t _ECG3_PAVG_2                       = 0x000000;
const uint32_t _ECG3_PAVG_4                       = 0x001000;
const uint32_t _ECG3_PAVG_8                       = 0x002000;
const uint32_t _ECG3_PAVG_16                      = 0x003000;

const uint8_t _ECG3_WRONG_ADDR                    = 0x01;
const uint8_t _ECG3_OK                            = 0x00;
const uint8_t _ECG3_INT_OCCURRED                  = 0x01;
const uint8_t _ECG3_INT_NOT_OCCURRED              = 0x00;
const uint8_t _ECG3_DUMMY_BYTE                    = 0x00;

void MAX30003_Write_Reg(uint8_t REG_ADDRESS, unsigned long data);
void MAX30003_Read_Reg(uint8_t REG_ADDRESS);
void MAX30003_ReadID(void);
void MAX30003_Init(int pin_miso, int pin_mosi, int pin_sck, int pin_cs );
//void max30003_spi_pre_transfer_callback(spi_transaction_t *t);
void MAX30003_Read_Data(void);
void Read_Data(void);
void Start_MAX30003(void);
void ecg3_getECG( uint32_t *outECG );
void plotECG(void);


/* ====================From Github===================== */



#endif /* MAIN_IC_DRIVER_MAX30003_H_ */
