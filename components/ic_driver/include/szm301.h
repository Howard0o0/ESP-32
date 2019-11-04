/*
 * author   :   howard
 * data     :   2019/09/26
 * descript :   driver of fingerprint sample module
*/

#ifndef __SZM301_H
#define __SZM301_H

#include <stdint.h>

/*
 * author       : howard
 * data         : 2019/09/28
 * instrucion   : fingerprint sample module driver
 *                uart2_install() first , then you can call any function bellow you want
*/

/* SZM301 connect ESP32 with uart2(IO14 IO12) */
void uart2_install(void);

/*
 * func : sample fingerprint character , len 199
 *        after call this function , module will wait finger. default waiting time is infinite, you can set waiting time by calling setSampleFingerprintTimeOut()
 * return : return NULL [fail] (waiting finger timeout or read fingerprint erro)
 *          return fingerprint character's addr [success]
*/
uint8_t *getFingerprintCharacter(void);

/*
 * func     : set max time waiting finger
 * param    : u8MaxTimeS : second
 * return   : success 0
 *            fail    -1    time out when waiting uart_rcv
 *                    -2    response's length from SZM301 illegal
 *                    -3    set time failed
*/
int setSampleFingerprintTimeOut(uint8_t u8MaxTimeS);

/*
 * func     : given a fingerprint character, sample a fingerprint charater, match two of them
 * param    : one of fingerprint character
 * return   :          0    matched
 *                    -1    unmatched
 *                    -2    time out when waiting uart_rcv
 *                    -3    response's length from SZM301 illegal
 *                    -4    wait finger time out
*/
int sampleFingerprintAndCmpCharacter(uint8_t *pu8Character);

/*
 * func     : set fingerprint match grade
 * param    : 0-9 , loose -> strict
 * return   :          0    success
 *                    -1    failed
 *                    -2    time out when waiting uart_rcv
 *                    -3    response's length from SZM301 illegal
*/
int setMatchGrade(uint8_t grade);

/*
 * func     : read fingerprint match grade
 * return   :         >0    fingerprint match grade
 *                    -1    failed
 *                    -2    time out when waiting uart_rcv
 *                    -3    response's length from SZM301 illegal
*/
int getMatchGrade(void);

#endif