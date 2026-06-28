// track.c
#include "track.h"



float error;
float normalized_error;




uint8_t line_digital[9]; // 线路状态（1-8个位置）

bool permanent_loss = false;   // 永久丢失标志
bool permanent_arrivel = false; //到达目的标志
bool permanent_Serch = false;   // 循迹中标志位


static pid_obj_t *Fiveline_serch_pid;  // 转向PID控制器
static pid_config_t Fiveline_serch_pid_config = INIT_PID_CONFIG(eightline_search_KP, eightline_search_KI, eightline_search_KD, eightline_search_INTEGRAL, eightline_search_MAX,
                                                                (PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement));

void Track_Init()
{
    //循迹pid控制器
    Fiveline_serch_pid = pid_register(&Fiveline_serch_pid_config);
}



// 滤波缓存，5路传感器，每路8次滑动平均
#define ADC_FILTER_CNT 8
uint16_t adc_raw_buf[5][ADC_FILTER_CNT];
//滑动平均模拟量
uint16_t line_analog_avg[9];
uint8_t filter_idx = 0;

// 读取ADC并滤波
void Five_line_GetDate_ADC(void)
{
    // 1. 读取5路ADC原始值
    adc_raw_buf[0][filter_idx] =  adc_buf[5];
    adc_raw_buf[1][filter_idx] =  adc_buf[2];
    adc_raw_buf[2][filter_idx] =  adc_buf[1];
    adc_raw_buf[3][filter_idx] =  adc_buf[3];
    adc_raw_buf[4][filter_idx] =  adc_buf[4];
    filter_idx = (filter_idx + 1) % ADC_FILTER_CNT;

    // 2. 滑动平均滤波
    for(int i=0; i<5; i++)
    {
        uint32_t sum = 0;
        for(int j=0; j<ADC_FILTER_CNT; j++)
        {
            sum += adc_raw_buf[i][j];
        }
        //映射到数组下标1-5
        line_analog_avg[i+1] = sum / ADC_FILTER_CNT;
    }
}
//数字量读取
void Five_line_GetDate()
{

    // 提取引脚状态
    line_digital[1] = HAL_GPIO_ReadPin(AD_XJ5_GPIO_Port,AD_XJ5_Pin);
    line_digital[2] = HAL_GPIO_ReadPin(AD_XJ2_GPIO_Port,AD_XJ2_Pin);
    line_digital[3] = HAL_GPIO_ReadPin(AD_XJ1_GPIO_Port,AD_XJ1_Pin);
    line_digital[4] = HAL_GPIO_ReadPin(AD_XJ3_GPIO_Port,AD_XJ3_Pin);
    line_digital[5] = HAL_GPIO_ReadPin(AD_XJ4_GPIO_Port,AD_XJ4_Pin);

    line_digital[0] = 1;
}





float line_following_control()
{
    static float last_error = 0.0f;      // 上次误差
    static uint8_t lost_line_count = 0;   // 丢失线路计数

    const float sensor_weights[5] = FIVESOR_WEIGHTS;




//    // 2. 检查永久丢失状态
//    if (permanent_loss) {
//        // 检查是否重新检测到线路
//        for (int i = 0; i < 5; i++) {
//            if (line_digital[i+1] == 1) {
//                // 重新检测到线路，恢复正常控制
//                permanent_loss = false;
//                lost_line_count = 0;
//                break;
//            }
//        }
//    }


    // 3. 计算线路位置（加权和）
    float weighted_sum = 0.0f;
    uint8_t active_sensors = 0;

    // 使用数字量数据（1-5位置）1为黑线，0为白线
    for (int i = 0; i < 5; i++) {
        if (line_digital[i+1] == 1) {
            weighted_sum += sensor_weights[i];
            active_sensors++;
        }
    }
    line_digital[0] = 0; //重置数据标志位，准备接收下一组数据

//    // 4. 处理丢失线路的情况
//    if (active_sensors == 0) {
//        lost_line_count++;
//
//        // 短暂丢失线路 - 使用上次误差继续控制
//        if (lost_line_count < LOST_LINE_TIMEOUT)
//        {
//            // 应用上次误差值
//            normalized_error = last_error /total_eror;
//            return;
//        }
//        else
//        {
//            // 设置永久丢失标志
//            permanent_loss = true;
//            lost_line_count = 0; // 重置计数
//
//            // 误差归零 直走
//            normalized_error = 0;
//            return;
//        }
//    }
//    else
//    {
//        lost_line_count = 0; // 线路重新出现，计数清零
//    }


    // 5. 计算当前位置偏差（-7~+7）
    float error = weighted_sum;//误差加权和
    last_error = error; // 保存当前误差

    // 6. 归一化误差值（-1.0~+1.0）
    normalized_error = error / total_eror;
    // 确保误差在合理范围内
    if (normalized_error > 1.0f) normalized_error = 1.0f;
    if (normalized_error < -1.0f) normalized_error = -1.0f;


    float line_serch_output = pid_calculate(Fiveline_serch_pid, normalized_error, 0);//pid计算输出差速比
    return line_serch_output;
}




