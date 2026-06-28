# SmartCar STM32 🚗

基于 STM32F103C8T6 的智能小车项目，支持**循迹**、**避障**、**PID 姿态控制**等多种模式。

## 硬件平台

| 部件 | 型号 |
|------|------|
| 主控 | STM32F103C8T6 (Cortex-M3, 72MHz) |
| IMU | ICM-42688-P (SPI) |
| 超声波 | HC-SR04 ×1 |
| 循迹 | 五路灰度传感器 (ADC) |
| 显示 | 0.96" OLED (I²C) |
| 电机 | 520编码电机 ×2 (PWM驱动) |
| 电池 | 7.4V 锂电池 → 电压监测 |

## 功能模块

```
Core/User/
├── Algorithm/
│   ├── Pid/              # PID 控制器 (速度环/转向环/自旋环)
│   └── Kalman_filter/    # 二维卡尔曼滤波器 (IMU角度融合)
├── Application/
│   ├── Car/              # 小车运动控制 (前进/转向/自旋/弧线)
│   └── Task/             # 任务调度管理层
└── Modules/
    ├── BatVolt/          # 电池电压检测
    ├── Buzzer/           # 无源蜂鸣器 (PWM音乐)
    ├── Delay/            # 微秒/毫秒延时
    ├── Dwt/              # DWT周期计数器
    ├── Key/              # 按键输入 (消抖)
    ├── LED/              # LED 呼吸灯
    ├── Motor/            # 编码电机驱动
    ├── MyI2C/            # 软件I²C
    ├── OLED/             # OLED 显示驱动
    ├── Task_Scheduler/   # 简易任务调度器
    ├── Track/            # 五路循迹传感器
    └── UltraSound/       # 超声波测距
```

## 运行模式

通过板载按键 KEY0 切换 4 种模式：

| 模式 | 功能组合 |
|:----:|----------|
| **1** | 超声波测距 + OLED显示 + 电量监测 |
| **2** | 全功能：循迹 + 避障 + 音乐 + PID控制 |
| **3** | 超声波 + 音乐 + PID自旋控制 |
| **4** | PID自旋控制 + OLED + 电量 |

## PID 控制架构

```
  target_angle ──→ [CarTurn_PID] ──→ diff_speed ──┐
                                                   ├──→ Motor[L/R]
  target_speed ──→ [Motor0/1_Speed_PID] ──────────┘
```

- **速度环**: 增量式PID，控制左右轮编码器转速
- **转向环**: 位置式PID，控制小车偏航角度
- **自旋环**: 位置式PID，控制原地旋转目标角度

关键参数位于 `Core/User/config.h`，详细调参参见 [配置说明](#配置参数)。

## 构建

### 前置条件

- **arm-none-eabi-gcc** 工具链
- CMake ≥ 3.28
- STM32CubeMX (可选，用于修改 `.ioc`)

### 编译

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

产物：
- `build/SmartCar.elf` — ELF可执行文件
- `build/SmartCar.hex` — HEX固件
- `build/SmartCar.bin` — 二进制固件

### 烧录

使用 ST-Link + OpenOCD：

```bash
openocd -f stlink.cfg -f target/stm32f1x.cfg -c "program build/SmartCar.elf verify reset exit"
```

或直接使用 STM32CubeProgrammer。

## 配置参数

`Core/User/config.h` 中可调节：

```c
// 循迹基础速度
#define XJ_BASE_SPEED      50

// 五路传感器权重
#define FIVESOR_WEIGHTS { -2.0, -1.0, 0, 1.0, 2.0 }

// 最大转向偏差
#define MAX_TURN           32

// 电机速度环
#define Motor0Speed_KP_V   40
#define Motor0Speed_KI_V   35

// 转向角度环
#define CarTurn_KP_V       2.0
#define CarTurn_KD_V       0.002

// 自旋角度环
#define CarSelfTurn_KP_A   0.5
#define CarSelfTurn_KD_A   0.002
```

## 项目结构

```
SmartCar_stm32/
├── CMakeLists.txt              # CMake构建配置
├── SmartCar.ioc                # CubeMX项目文件
├── STM32F103C8TX_FLASH.ld      # 链接脚本
├── stlink.cfg                  # OpenOCD配置
├── Core/
│   ├── Inc/                    # HAL外设头文件
│   ├── Src/                    # HAL外设源文件 + main.c
│   ├── Startup/                # 启动汇编
│   └── User/                   # 用户应用代码 (核心)
└── Drivers/
    └── STM32F1xx_HAL_Driver/   # STM32 HAL库
```

## License

MIT License

---

Author: [@Chenshuo515](https://github.com/Chenshuo515)
