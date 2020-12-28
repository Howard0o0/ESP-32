#include "fpga.h"
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
