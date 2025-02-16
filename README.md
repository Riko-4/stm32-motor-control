# Motor Control System with PWM, Direction Reversal, and Ultrasonic Sensor Integration

## Project Overview

This project is a motor control system implemented on an **STM32 microcontroller**, featuring **Pulse Width Modulation (PWM)** for speed control, **direction reversal**, and an **HC-SR04 ultrasonic sensor** for obstacle detection. The system allows three motors to operate at variable speeds, with automatic stopping or speed adjustments based on distance readings from the ultrasonic sensor. The project is implemented using the **STM32 HAL library** with **STM32CubeIDE**.

---

## Features

- **PWM-based Motor Speed Control**: Control the speed of three motors using PWM.
- **Direction Reversal**: Motors can change direction by modifying PWM values and GPIO states.
- **Timer-based Control**: Utilizes STM32's timers (TIM1, TIM2, TIM3) for precise PWM generation.
- **Ultrasonic Sensor Integration**: Uses the **HC-SR04** to measure distance and adjust motor behavior accordingly.
- **Automatic Obstacle Detection & Response**:
  - **If distance > 10cm** → Motors run at varying speeds.
  - **If distance ≤ 10cm** → Motors stop to prevent collisions.
  - **Gradual Speed Adjustment** for smooth acceleration.

---

## Hardware Requirements

### **Microcontroller**
- **STM32F103C8T6 (Blue Pill)** or any STM32 with timer & PWM support.

### **Components**
| Component | Description |
|-----------|-------------|
| STM32 Board | Main microcontroller |
| HC-SR04 | Ultrasonic distance sensor |
| L298N | Motor driver (optional, if using DC motors) |
| DC Motors | Used for movement |
| Power Supply | 5V for STM32, 12V for motors |

### **Wiring Connections**

#### **HC-SR04 to STM32**
| HC-SR04 Pin | STM32 Pin |
|------------|----------|
| VCC | 5V |
| GND | GND |
| Trig | PA8 |
| Echo | PA9 |

#### **Motors (via PWM)**
| Motor | STM32 PWM Pin |
|-------|-------------|
| Motor 1 | PA11 |
| Motor 2 | PA12 |
| Motor 3 | PA13 |

## How It Works

### **1️⃣ PWM-Based Motor Control**
- Motors are controlled using **PWM signals** on **TIM1, TIM2, and TIM3**.
- Each motor’s speed is adjusted dynamically using the duty cycle.
  - **Motor 1**: Initially set to 50% speed.
  - **Motor 2**: Initially set to 75% speed.
  - **Motor 3**: Initially set to 25% speed.
- The direction of rotation is controlled by toggling **GPIO pins (PA11, PA12, PA13)**.

### **2️⃣ Ultrasonic Sensor Distance Measurement**
- The **HC-SR04** sends out an ultrasonic pulse via the `Trig` pin.
- The `Echo` pin receives the reflected signal.
- The time difference is used to calculate the **distance to an obstacle**.

### **3️⃣ Motor Speed Adjustment Based on Distance**
- If **distance > 10cm**, motors run normally.
- If **distance ≤ 10cm**, motors **stop to prevent collisions**.
- Speed **gradually increases** when obstacles move away.

---

4. **GPIO Control**:
   - Motor direction is controlled using GPIO pins (PA11, PA12, PA13) which are set to `HIGH` or `LOW` based on the required motor direction.

## Installation and Setup

1. **Install STM32CubeIDE**: If you haven’t already, download and install STM32CubeIDE from the [STMicroelectronics website](https://www.st.com/en/development-tools/stm32cubeide.html).
   
2. **Clone the Repository**

## Further Additions or Improvements
While the current version provides a solid foundation, there are numerous ways you can improve and expand the functionality of the motor control system. This is just a preliminary base to learn the ARM Microcontroller architectures.
1. ** Wireless Control**
2. **Encoder Integration for Closed-Loop Control**
3. **Advanced Motor Control Algorithms**
4. **Real-time Monitoring and Diagnostics**
5. **Buzzer/LED Alerts when obstacle is detected**
And many more.
