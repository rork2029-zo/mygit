// (키, 평문 입력 받아서) 함수들 돌려서 키, 평문, 암호문, 복호화 결과 출력
/* Test vectors 2 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "HIGHT.h"

int main(){
    uint8_t MK[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};  // 마스터키
    uint8_t PT[8] = {0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00}; // 평문
    uint8_t CT[8] = {0x00, }; // 암호문
    uint8_t DT[8] = {0x00, }; // 복호문

    printf("Key : ");
    for(int i = 15; i>=0; i--)   printf("%x ", MK[i]);
    printf("\nPlaintext : ");
    for(int i = 7; i>=0; i--)    printf("%x ", PT[i]);
    printf("\n");

    /* Encryption */
    enc(CT, PT, MK);

    printf("[Cipher Text]\n");
    for (int i = 7; i>=0; i--){
        printf("%x ", CT[i]);
    }

    /* Decryption */
    dec(DT, CT, MK);

    printf("\n[Decrypted Text]\n");
    for (int i = 7; i>=0; i--){
        printf("%x ", DT[i]);
    }

    return 0;
}