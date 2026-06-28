#include "kalman_filter.h"
#include <math.h>

// 初始化二维卡尔曼滤波器
KalmanFilter2D* Kalman2DInit(float init_x[2], float init_P[4], float Q[4], float R[4]) {
    KalmanFilter2D* kf = (KalmanFilter2D*)malloc(sizeof(KalmanFilter2D));
    if (kf == NULL) return NULL;

    // 初始化状态向量
    kf->x[0] = init_x[0];
    kf->x[1] = init_x[1];

    // 初始化协方差矩阵
    for (int i = 0; i < 4; i++) {
        kf->P[i] = init_P[i];
    }

    // 初始化过程噪声协方差
    for (int i = 0; i < 4; i++) {
        kf->Q[i] = Q[i];
    }

    // 初始化测量噪声协方差
    for (int i = 0; i < 4; i++) {
        kf->R[i] = R[i];
    }

    // 设置默认状态转移矩阵 (单位矩阵)
    kf->F[0] = 1.0f; kf->F[1] = 0.0f;
    kf->F[2] = 0.0f; kf->F[3] = 1.0f;

    // 设置默认测量矩阵 (单位矩阵)
    kf->H[0] = 1.0f; kf->H[1] = 0.0f;
    kf->H[2] = 0.0f; kf->H[3] = 1.0f;

    return kf;
}

// 设置状态转移矩阵（考虑时间步长dt）
// 对于位置-速度模型：F = [1 dt; 0 1]
void Kalman2DSetDT(KalmanFilter2D* kf, float dt) {
    if (kf == NULL) return;

    kf->F[0] = 1.0f;
    kf->F[1] = dt;
    kf->F[2] = 0.0f;
    kf->F[3] = 1.0f;
}

// 矩阵乘法辅助函数 (2x2 * 2x2)
static void mat_mult_2x2(const float A[4], const float B[4], float result[4]) {
    result[0] = A[0]*B[0] + A[1]*B[2];
    result[1] = A[0]*B[1] + A[1]*B[3];
    result[2] = A[2]*B[0] + A[3]*B[2];
    result[3] = A[2]*B[1] + A[3]*B[3];
}

// 矩阵乘法辅助函数 (2x2 * 2x1)
static void mat_mult_2x2_2x1(const float A[4], const float B[2], float result[2]) {
    result[0] = A[0]*B[0] + A[1]*B[1];
    result[1] = A[2]*B[0] + A[3]*B[1];
}

// 矩阵转置 (2x2)
static void mat_transpose_2x2(const float A[4], float result[4]) {
    result[0] = A[0];
    result[1] = A[2];
    result[2] = A[1];
    result[3] = A[3];
}

// 矩阵加法 (2x2)
static void mat_add_2x2(const float A[4], const float B[4], float result[4]) {
    for (int i = 0; i < 4; i++) {
        result[i] = A[i] + B[i];
    }
}

// 矩阵减法 (2x2)
static void mat_sub_2x2(const float A[4], const float B[4], float result[4]) {
    for (int i = 0; i < 4; i++) {
        result[i] = A[i] - B[i];
    }
}
// 卡尔曼滤波更新（独立状态版本）
void  Kalman2DUpdateSeparate(KalmanFilter2D* kf, float measurement0, float measurement1) {
    if (kf == NULL) return;

    // 创建临时测量数组
    float measurement[2] = {measurement0, measurement1};

    // ======================= 预测步骤 =======================
    // 状态预测: x_pred = F * x
    float x_pred[2];
    mat_mult_2x2_2x1(kf->F, kf->x, x_pred);

    // 协方差预测: P_pred = F * P * F^T + Q
    float FP[4], FPFT[4], P_pred[4];
    mat_mult_2x2(kf->F, kf->P, FP);

    float FT[4];
    mat_transpose_2x2(kf->F, FT);
    mat_mult_2x2(FP, FT, FPFT);

    mat_add_2x2(FPFT, kf->Q, P_pred);

    // ======================= 更新步骤 =======================
    // 计算残差: y = z - H * x_pred
    float Hx_pred[2];
    mat_mult_2x2_2x1(kf->H, x_pred, Hx_pred);

    float y[2] = {
        measurement[0] - Hx_pred[0],
        measurement[1] - Hx_pred[1]
    };

    // 计算残差协方差: S = H * P_pred * H^T + R
    float HP_pred[4], HP_predHT[4], S[4];
    mat_mult_2x2(kf->H, P_pred, HP_pred);

    float HT[4];
    mat_transpose_2x2(kf->H, HT);
    mat_mult_2x2(HP_pred, HT, HP_predHT);

    mat_add_2x2(HP_predHT, kf->R, S);

    // 计算卡尔曼增益: K = P_pred * H^T * inv(S)
    float P_predHT[4];
    mat_mult_2x2(P_pred, HT, P_predHT);

    // 计算S的逆 (2x2矩阵求逆)
    float det = S[0]*S[3] - S[1]*S[2];
    if (fabsf(det) < 1e-7) det = 1e-7; // 防止除零

    float S_inv[4] = {
        S[3]/det, -S[1]/det,
       -S[2]/det,  S[0]/det
   };

    float K[4]; // 卡尔曼增益矩阵 (2x2)
    mat_mult_2x2(P_predHT, S_inv, K);

    // 更新状态估计: x = x_pred + K * y
    float Ky[2];
    mat_mult_2x2_2x1(K, y, Ky);

    kf->x[0] = x_pred[0] + Ky[0];
    kf->x[1] = x_pred[1] + Ky[1];

    // 更新协方差: P = (I - K * H) * P_pred
    float KH[4], I_minus_KH[4];
    mat_mult_2x2(K, kf->H, KH);

    float I[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    mat_sub_2x2(I, KH, I_minus_KH);

    mat_mult_2x2(I_minus_KH, P_pred, kf->P);
}
// 卡尔曼滤波更新
void Kalman2DUpdate(KalmanFilter2D* kf, float measurement[2]) {
    if (kf == NULL) return;

    // ======================= 预测步骤 =======================
    // 状态预测: x_pred = F * x
    float x_pred[2];
    mat_mult_2x2_2x1(kf->F, kf->x, x_pred);

    // 协方差预测: P_pred = F * P * F^T + Q
    float FP[4], FPFT[4], P_pred[4];
    mat_mult_2x2(kf->F, kf->P, FP);

    float FT[4];
    mat_transpose_2x2(kf->F, FT);
    mat_mult_2x2(FP, FT, FPFT);

    mat_add_2x2(FPFT, kf->Q, P_pred);

    // ======================= 更新步骤 =======================
    // 计算残差: y = z - H * x_pred
    float Hx_pred[2];
    mat_mult_2x2_2x1(kf->H, x_pred, Hx_pred);

    float y[2] = {
        measurement[0] - Hx_pred[0],
        measurement[1] - Hx_pred[1]
    };

    // 计算残差协方差: S = H * P_pred * H^T + R
    float HP_pred[4], HP_predHT[4], S[4];
    mat_mult_2x2(kf->H, P_pred, HP_pred);

    float HT[4];
    mat_transpose_2x2(kf->H, HT);
    mat_mult_2x2(HP_pred, HT, HP_predHT);

    mat_add_2x2(HP_predHT, kf->R, S);

    // 计算卡尔曼增益: K = P_pred * H^T * inv(S)
    float P_predHT[4];
    mat_mult_2x2(P_pred, HT, P_predHT);

    // 计算S的逆 (2x2矩阵求逆)
    float det = S[0]*S[3] - S[1]*S[2];
    if (fabsf(det) < 1e-7) det = 1e-7; // 防止除零

    float S_inv[4] = {
         S[3]/det, -S[1]/det,
        -S[2]/det,  S[0]/det
    };

    float K[4]; // 卡尔曼增益矩阵 (2x2)
    mat_mult_2x2(P_predHT, S_inv, K);

    // 更新状态估计: x = x_pred + K * y
    float Ky[2];
    mat_mult_2x2_2x1(K, y, Ky);

    kf->x[0] = x_pred[0] + Ky[0];
    kf->x[1] = x_pred[1] + Ky[1];

    // 更新协方差: P = (I - K * H) * P_pred
    float KH[4], I_minus_KH[4];
    mat_mult_2x2(K, kf->H, KH);

    float I[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    mat_sub_2x2(I, KH, I_minus_KH);

    mat_mult_2x2(I_minus_KH, P_pred, kf->P);
}

// 获取当前状态估计
void Kalman2DGetState(const KalmanFilter2D* kf, float state[2]) {
    if (kf == NULL || state == NULL) return;
    state[0] = kf->x[0];
    state[1] = kf->x[1];
}

// 获取当前协方差
void Kalman2DGetCovariance(const KalmanFilter2D* kf, float cov[4]) {
    if (kf == NULL || cov == NULL) return;
    for (int i = 0; i < 4; i++) {
        cov[i] = kf->P[i];
    }
}

// 释放滤波器资源
void Kalman2DFree(KalmanFilter2D* kf) {
    if (kf != NULL) {
        free(kf);
    }
}