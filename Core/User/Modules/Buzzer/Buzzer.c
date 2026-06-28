//
// Created by 陈烁 on 25-2-23.
//
//
// Created by 陈烁 on 25-2-23.
// 改造：支持动态修改定时器频率(音调) + 音量 + 节奏
//
#include "Buzzer.h"


// 定时器基础参数：根据你TIM1时钟配置填写！
// 例：TIM1 时钟 = 72MHz
#define BUZZER_TIM_CLOCK     72000000UL
// TIM预分频器(PSC)，和CubeMX配置保持一致
#define BUZZER_TIM_PSC       71U

/**
 * @brief  PWM控制蜂鸣器：设置 频率(音调) + 占空比(音量)
 * @param  freq  : 发声频率(Hz)，音调，可自由修改 200~2000
 * @param  duty  : 占空比 0~99，音量
 * @retval 无
 */
void Buzzer_ON(uint16_t freq, uint8_t duty)
{
    if(freq < 260)  freq = 260;   // 下限抬高，避免过低浑浊音
    if(freq > 1000) freq = 2000;  // 上限压低，避免尖锐刺耳
    if(duty > 99)   duty = 99;
    if(duty < 20)   duty = 20;    // 最低音量限制，防止无声

    uint32_t arr = (BUZZER_TIM_CLOCK / (BUZZER_TIM_PSC + 1)) / freq - 1;
    __HAL_TIM_SET_AUTORELOAD(&htim1, arr);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, (arr * duty) / 100);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

/**
 * @brief  关闭蜂鸣器
 */
void Buzzer_OFF(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
}



/************************ 兰花草 1=E 4/4 ?=96 ************************
 * 格式：{频率(Hz), 发声时长ms, 停顿时长ms, 音量0~99}
 * 音高基准：中音1(Do)=E4=330Hz  十二平均律
 *********************************************************************/
static const uint16_t song_data[][4] = {
        // 第1小节：我从山中来  低音6 3 3 3 3·2
        {277, 310, 30, 72},  // 低音6(La) 八分
        {415, 310, 30, 78},  // 中音3(Mi) 八分
        {415, 310, 30, 78},  // 中音3(Mi) 八分
        {415, 310, 30, 78},  // 中音3(Mi) 八分
        {415, 940, 50, 80},  // 中音3(Mi) 附点四分
        {370, 310, 30, 76},  // 中音2(Re) 八分

        // 第2小节：带着兰花草  1·2 1 7 6 -
        {330, 470, 20, 78},  // 中音1(Do) 附点八分
        {370, 160, 30, 75},  // 中音2(Re) 十六分
        {330, 310, 30, 76},  // 中音1(Do) 八分
        {622, 310, 30, 74},  // 中音7(Si) 八分
        {554, 1250, 80, 72}, // 中音6(La) 二分长音

        // 第3小节：种在小园中  6 6 6 6 6·5
        {554, 310, 30, 78},  // 中音6(La) 八分
        {554, 310, 30, 78},  // 中音6(La) 八分
        {554, 310, 30, 78},  // 中音6(La) 八分
        {554, 310, 30, 78},  // 中音6(La) 八分
        {554, 940, 50, 80},  // 中音6(La) 附点四分
        {494, 310, 30, 76},  // 中音5(Sol) 八分

        // 第4小节：希望花开早  3 5 5#4 3 -
        {415, 310, 30, 78},  // 中音3(Mi) 八分
        {494, 310, 30, 78},  // 中音5(Sol) 八分
        {494, 310, 30, 78},  // 中音5(Sol) 八分
        {466, 310, 30, 76},  // 中音升4(#Fa) 八分
        {415, 1250, 80, 72}, // 中音3(Mi) 二分长音

        // 第5小节：一日看三回  3 6 6 5 3·2
        {415, 310, 30, 78},  // 中音3(Mi) 八分
        {554, 310, 30, 78},  // 中音6(La) 八分
        {554, 310, 30, 78},  // 中音6(La) 八分
        {494, 310, 30, 76},  // 中音5(Sol) 八分
        {415, 940, 50, 80},  // 中音3(Mi) 附点四分
        {370, 310, 30, 76},  // 中音2(Re) 八分

        // 第6小节：看的花时过  1·2 1 7 低音6 低音3
        {330, 470, 20, 78},  // 中音1(Do) 附点八分
        {370, 160, 30, 75},  // 中音2(Re) 十六分
        {330, 310, 30, 76},  // 中音1(Do) 八分
        {622, 310, 30, 74},  // 中音7(Si) 八分
        {277, 310, 30, 68},  // 低音6(La) 八分
        {208, 310, 30, 65},  // 低音3(Mi) 八分

        // 第7小节：兰花却依然  低音3 1 1 7 低音6·低音3
        {208, 310, 30, 65},  // 低音3(Mi) 八分
        {330, 310, 30, 76},  // 中音1(Do) 八分
        {330, 310, 30, 76},  // 中音1(Do) 八分
        {622, 310, 30, 74},  // 中音7(Si) 八分
        {277, 940, 50, 70},  // 低音6(La) 附点四分
        {208, 310, 30, 65},  // 低音3(Mi) 八分

        // 第8小节：苞也无一个  2 1 7 5 6 -  结尾收尾
        {370, 310, 30, 76},  // 中音2(Re) 八分
        {330, 310, 30, 75},  // 中音1(Do) 八分
        {622, 310, 30, 74},  // 中音7(Si) 八分
        {494, 310, 30, 72},  // 中音5(Sol) 八分
        {554, 1250, 600, 70} // 中音6(La) 二分结尾 长停顿
};
#define SONG_NOTE_COUNT  (sizeof(song_data) / sizeof(song_data[0]))



static SongState_t song_state = SONG_IDLE;
static uint8_t    curr_note_idx = 0;
static uint16_t   song_start_tick = 0;

/**
 * @brief  启动歌曲播放
 */
void Buzzer_Start_StarSong(void)
{
    if(SONG_IDLE == song_state)
    {
        curr_note_idx = 0;
        song_state    = SONG_PLAY_NOTE;
        song_start_tick = Flag_10ms_Cnt;
        // 开启第一个音符：频率+音量
        Buzzer_ON(song_data[curr_note_idx][0], song_data[curr_note_idx][3]);
    }
}

/**
 * @brief  状态机轮询函数 【必须10ms定时器中断内循环调用】
 */
void Buzzer_Song_State_Machine(void)
{
    if(SONG_IDLE == song_state)
        return;

    uint16_t tick_gap = Flag_10ms_Cnt - song_start_tick;
    uint16_t curr_ms  = tick_gap * 10;  // 1tick = 10ms

    switch(song_state)
    {
        case SONG_PLAY_NOTE:
        {
            uint16_t play_ms = song_data[curr_note_idx][1];
            if(curr_ms >= play_ms)
            {
                Buzzer_OFF();
                song_state = SONG_PAUSE_NOTE;
                song_start_tick = Flag_10ms_Cnt;
            }
            break;
        }

        case SONG_PAUSE_NOTE:
        {
            uint16_t pause_ms = song_data[curr_note_idx][2];
            if(curr_ms >= pause_ms)
            {
                curr_note_idx++;
                if(curr_note_idx >= SONG_NOTE_COUNT)
                {
                    // 播放完毕
                    song_state = SONG_IDLE;
                    Buzzer_OFF();
                }
                else
                {
                    // 下一个音符
                    song_state = SONG_PLAY_NOTE;
                    song_start_tick = Flag_10ms_Cnt;
                    Buzzer_ON(song_data[curr_note_idx][0], song_data[curr_note_idx][3]);
                }
            }
            break;
        }

        default:
            song_state = SONG_IDLE;
            Buzzer_OFF();
            break;
    }
}

/**
 * @brief  强制停止歌曲播放
 */
void Buzzer_Stop_Song(void)
{
    song_state = SONG_IDLE;
    Buzzer_OFF();
}