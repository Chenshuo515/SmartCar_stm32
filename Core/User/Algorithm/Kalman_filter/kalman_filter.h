#ifndef KALMAN_FILTER_2D_H
#define KALMAN_FILTER_2D_H

#include <stdlib.h>

// 二维卡尔曼滤波器结构体
typedef struct {
    // 状态向量 [x, y]^T
    float x[2];

    // 协方差矩阵 (2x2)
    // [P[0] P[1]]
    // [P[2] P[3]]
    float P[4];

    // 状态转移矩阵 (2x2)
    float F[4];

    // 过程噪声协方差矩阵 (2x2)
    float Q[4];

    // 测量噪声协方差矩阵 (2x2)
    float R[4];

    // 测量矩阵 (2x2)
    float H[4];
} KalmanFilter2D;

// 初始化二维卡尔曼滤波器
KalmanFilter2D* Kalman2DInit(float init_x[2], float init_P[4], float Q[4], float R[4]);

// 设置状态转移矩阵（考虑时间步长dt）
void Kalman2DSetDT(KalmanFilter2D* kf, float dt);

// 卡尔曼滤波更新
void Kalman2DUpdate(KalmanFilter2D* kf, float measurement[2]);
void Kalman2DUpdateSeparate(KalmanFilter2D* kf, float measurement0, float measurement1);
// 获取当前状态估计
void Kalman2DGetState(const KalmanFilter2D* kf, float state[2]);;

// 获取当前协方差
void Kalman2DGetCovariance(const KalmanFilter2D* kf, float cov[4]);

// 释放滤波器资源
void Kalman2DFree(KalmanFilter2D* kf);

#endif // KALMAN_FILTER_2D_H