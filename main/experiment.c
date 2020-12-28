#include "fpga.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "szm301.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getPositiveBitCnt(int x) {
	int cnt = 0;
	while (x != 0) {
		cnt++;
		x &= x - 1;
	}
	return cnt;
}

int hammingDistance(int x, int y) {
	return getPositiveBitCnt(x ^ y);
}

int CalcHamingDist(const uint8_t* x, const uint8_t* y, int len) {
	int dist = 0;
	for (int i = 0; i < len; i++)
		dist += hammingDistance(x[ i ], y[ i ]);
	return dist;
}

void FRR(int round) {

	uint8_t* pu8PufResp	  = ( uint8_t* )calloc(20 + 1, sizeof(uint8_t));
	uint8_t* standardRsp	  = ( uint8_t* )calloc(20 + 1, sizeof(uint8_t));
	uint8_t* pu8Fingerprint	  = NULL;
	int	 reject_cnt[ 10 ] = { 0 };

	pu8Fingerprint = getFingerprintCharacter();

	for (int i = 0; i < round; i++) {
		printf("round %d \r\n", i);

		if (!pu8Fingerprint) {
			printf("getFingerprintCharacter failed\r\n");
			continue;
		}

		int result = finigerprintTo20bytesResponse(pu8Fingerprint, pu8PufResp);
		if (result != 0) {
			printf("finigerprintTo20bytesResponse failed\r\n");
			continue;
		}

		printf("puf response : ");
		print_hex(( char* )pu8PufResp, 20);

		if (standardRsp[ 0 ] == 0 && standardRsp[ 1 ] == 0)
			memcpy(standardRsp, pu8PufResp, 20);

		int dist = CalcHamingDist(standardRsp, pu8PufResp, 20);
		if (dist >= 10)
			printf("a too large dist : %d !\r\n", dist);

		for (int i = 0; i < 10 && i < dist; i++)
			reject_cnt[ i ]++;
	}

	for (int thresh = 0; thresh < 10; thresh++) {
		float frr = ( float )reject_cnt[ thresh ] / round;
		printf("thresh : %d , frr : %.3f \r\n", thresh, frr);
	}

	printf("test done!\r\n");
}

void FARByDifferentFinger(int finger_cnt, int round) {

	uint8_t* pu8PufResp	   = ( uint8_t* )calloc(20 + 1, sizeof(uint8_t));
	uint8_t* standardRsp	   = ( uint8_t* )calloc(20 + 1, sizeof(uint8_t));
	uint8_t* pu8Fingerprint	   = NULL;
	int	 accept_cnt[ 162 ] = { 0 };

	// get standard finger and response
	while (!pu8Fingerprint || standardRsp[ 0 ] == 0) {
		pu8Fingerprint = getFingerprintCharacter();
		if (!pu8Fingerprint) {
			printf("get standard FingerprintCharacter failed\r\n");
			continue;
		}

		int result = finigerprintTo20bytesResponse(pu8Fingerprint, standardRsp);
		if (result != 0) {
			printf("standard finigerprintTo20bytesResponse failed\r\n");
			continue;
		}

		printf("standard puf response : ");
		print_hex(( char* )standardRsp, 20);
	}

	for (int i = 0; i < finger_cnt; i++) {
		printf("waiting fake fingerprint %d ...\r\n", finger_cnt);
		vTaskDelay(2000 / portTICK_RATE_MS);

		pu8Fingerprint = getFingerprintCharacter();
		if (!pu8Fingerprint) {
			printf("get standard FingerprintCharacter failed\r\n");
			continue;
		}

		// 每个指纹测round轮，测10个指纹
		for (int i = 0; i < round; i++) {

			int result = finigerprintTo20bytesResponse(pu8Fingerprint, pu8PufResp);
			if (result != 0) {
				printf("standard finigerprintTo20bytesResponse failed\r\n");
				continue;
			}

			int dist = CalcHamingDist(standardRsp, pu8PufResp, 20);

			for (int i = dist + 1; i < 160; i++)
				accept_cnt[ i ]++;
		}
	}

	for (int thresh = 0; thresh < 160; thresh++) {
		float frr = ( float )accept_cnt[ thresh ] / round;
		printf("thresh : %d , frr : %.3f \r\n", thresh, frr);
	}

	printf("test done!\r\n");
}

void getPUFResponse() {
	int	iRet;
	uint8_t au8Resp[ 21 ];
	uint8_t challenge[ 200 ] = { 0 };

	while (1) {
		iRet = finigerprintTo20bytesResponse(challenge, au8Resp);
		if (iRet != 0) {
			printf(" get R failed \r\n");
		}
		else {
			printf("R: \r\n");
			print_hex(( char* )au8Resp, 20);
		}
	}
}

void FARByDifferentPUFBoard(int round) {

	uint8_t* pu8PufResp	       = ( uint8_t* )calloc(20 + 1, sizeof(uint8_t));
	uint8_t	 standardRsp[]	       = {};
	uint8_t	 pu8Fingerprint[ 200 ] = { 0 };
	int	 accept_cnt[ 162 ]     = { 0 };

	for (int i = 0; i < round; i++) {

		int result = finigerprintTo20bytesResponse(pu8Fingerprint, pu8PufResp);
		if (result != 0) {
			printf("standard finigerprintTo20bytesResponse failed\r\n");
			continue;
		}

		int dist = CalcHamingDist(standardRsp, pu8PufResp, 20);

		for (int i = dist + 1; i < 160; i++)
			accept_cnt[ i ]++;
	}

	for (int thresh = 0; thresh < 160; thresh++) {
		float frr = ( float )accept_cnt[ thresh ] / round;
		printf("thresh : %d , frr : %.3f \r\n", thresh, frr);
	}

	printf("test done!\r\n");
}
