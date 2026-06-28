//
// Created by 陈烁 on 25-2-23.
//

#ifndef BUZZER_H
#define BUZZER_H
#include "main.h"
#include "tim.h"
#include "stdint.h"

extern uint16_t Flag_10ms_Cnt;
typedef enum {
    SONG_IDLE,
    SONG_PLAY_NOTE,
    SONG_PAUSE_NOTE
} SongState_t;




// 蜂鸣器基础控制
void Buzzer_ON(uint16_t freq, uint8_t duty);  // freq:频率(Hz), duty:占空比0~99
void Buzzer_OFF(void);

// 歌曲播放控制
void Buzzer_Start_StarSong(void);
void Buzzer_Song_State_Machine(void);
void Buzzer_Stop_Song(void);


#endif //BUZZER_H
