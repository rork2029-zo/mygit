#include "HIGHT.h"

uint8_t delta[128] = 
{ 0x5a, 0x6d, 0x36, 0x1b, 0xd, 0x6, 0x3, 0x41, 
0x60, 0x30, 0x18, 0x4c, 0x66, 0x33, 0x59, 0x2c,
0x56, 0x2b, 0x15, 0x4a, 0x65, 0x72, 0x39, 0x1c,
0x4e, 0x67, 0x73, 0x79, 0x3c, 0x5e, 0x6f, 0x37,
0x5b, 0x2d, 0x16, 0xb, 0x5, 0x42, 0x21, 0x50,
0x28, 0x54, 0x2a, 0x55, 0x6a, 0x75, 0x7a, 0x7d,
0x3e, 0x5f, 0x2f, 0x17, 0x4b, 0x25, 0x52, 0x29,
0x14, 0xa, 0x45, 0x62, 0x31, 0x58, 0x6c, 0x76,
0x3b, 0x1d, 0xe, 0x47, 0x63, 0x71, 0x78, 0x7c,
0x7e, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x43, 0x61,
0x70, 0x38, 0x5c, 0x6e, 0x77, 0x7b, 0x3d, 0x1e,
0x4f, 0x27, 0x53, 0x69, 0x34, 0x1a, 0x4d, 0x26,
0x13, 0x49, 0x24, 0x12, 0x9, 0x4, 0x2, 0x1,
0x40, 0x20, 0x10, 0x8, 0x44, 0x22, 0x11, 0x48,
0x64, 0x32, 0x19, 0xc, 0x46, 0x23, 0x51, 0x68, 
0x74, 0x3a, 0x5d, 0x2e, 0x57, 0x6b, 0x35, 0x5a };

/* Enc */
void enc(uint8_t CT[], uint8_t PT[], uint8_t MK[]){
    uint8_t wk[8] = {0x00, };
    uint8_t sk[128] = {0x00, };
    //uint8_t delta[128] = {0x00, };

    // make subkey ===============================
    for(int i = 0; i<=3; i++)   wk[i] = MK[i+12];
    for(int i = 4; i<=7; i++)   wk[i] = MK[i-4];

    // 사전에 계산한 테이블 사용 시 주석처리할 단락
    // delta[0] = 0x5a;
    // uint8_t a;
    // for (int i = 1; i < 128; i++){
    //     uint8_t tmp = delta[i-1];
    //     a = (0x01 & delta[i-1]) ^ ((0x08 & delta[i-1]) >> 3);
    //     delta[i] = (a << 6) | (tmp >> 1);
    // }
    //

    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            sk[16*i+j] = (MK[(j-i)&7] + delta[16*i+j]);
        }
        for(int j = 0; j<8; j++){
            sk[16*i+j+8] = (MK[((j-i)&7)+8] + delta[16*i+j+8]);
        }    
    }
    // ============================================


    uint8_t X[8];
    memcpy(X, PT, 8);

    // 초기변환
    X[0] += wk[0];
    X[2] ^= wk[1];
    X[4] += wk[2];
    X[6] ^=wk[3];

    // 1~31-round
    for(int i = 1; i<32; i++){
        // X[i][j] = X[i-1][j-1], j = 1, 3, 5, 7
        uint8_t tmp1 = X[7], tmp2 = X[6];
        X[7] = X[6];
        X[6] = (X[5] + (f1(X[4])^sk[4*i-2]));
        X[5] = X[4];
        X[4] = X[3] ^ ((f0(X[2])+sk[4*i-3]));
        X[3] = X[2];
        X[2] = (X[1] + (f1(X[0])^sk[4*i-4]));
        X[1] = X[0];
        X[0] = tmp1 ^ ((f0(tmp2)+sk[4*i-1]));
    }

    // 32-round
    X[1] += (f1(X[0])^sk[124]);
    X[3] ^= ((f0(X[2])+sk[125]));
    X[5] += (f1(X[4])^sk[126]);
    X[7] ^= ((f0(X[6])+sk[127]));

    // 최종변환
    X[0] += wk[4];
    X[2] ^= wk[5];
    X[4] += wk[6];
    X[6] ^= wk[7];

    memcpy(CT, X, 8);
}

/* Dec */
void dec(uint8_t DT[], uint8_t CT[], uint8_t MK[]){

    uint8_t wk[8] = {0x00, };
    uint8_t sk[128] = {0x00, };
    //uint8_t delta[128] = {0x00, };

    // make subkey ===============================
    for(int i = 0; i<=3; i++)   wk[i] = MK[i+12];
    for(int i = 4; i<=7; i++)   wk[i] = MK[i-4];

    // delta[0] = 0x5a;
    // uint8_t a;

    // for (int i = 1; i < 128; i++){
    //     uint8_t tmp = delta[i-1];
    //     a = (0x01 & delta[i-1]) ^ ((0x08 & delta[i-1]) >> 3);
    //     delta[i] = (a << 6) | (tmp >> 1);
    // }

    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            sk[16*i+j] = (MK[(j-i)&7] + delta[16*i+j]);
        }
        for(int j = 0; j<8; j++){
            sk[16*i+j+8] = (MK[((j-i)&7)+8] + delta[16*i+j+8]);
        }    
    }
    // ============================================

    uint8_t X[8];
    memcpy(X, CT, 8);

    X[0] -= wk[4];
    X[2] ^= wk[5];
    X[4] -= wk[6];
    X[6] ^= wk[7];

    for(int i = 1; i<32; i++){
        // X[i][j] = X[i-1][j-1], j = 1, 3, 5, 7
        uint8_t tmp1 = X[1], tmp3 = X[3], tmp5 = X[5], tmp7 = X[7];
        X[1] = X[2];
        X[3] = X[4];
        X[5] = X[6];
        X[7] = X[0];

        X[0] = tmp1 - (f1(X[0])^sk[127-(4*i-1)]);
        X[2] = tmp3 ^ ((f0(X[2])+sk[127-(4*i-2)]));
        X[4] = tmp5 - (f1(X[4])^sk[127-(4*i-3)]);
        X[6] = tmp7 ^ ((f0(X[6])+sk[127-(4*i-4)]));
    }

    // 32-round
    X[1] -= (f1(X[0])^sk[0]);
    X[3] ^= ((f0(X[2])+sk[1]));
    X[5] -= (f1(X[4])^sk[2]);
    X[7] ^= ((f0(X[6])+sk[3]));

    // 최종변환
    X[0] -= wk[0];
    X[2] ^= wk[1];
    X[4] -= wk[2];
    X[6] ^= wk[3];

    memcpy(DT, X, 8);
}
