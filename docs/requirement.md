# 图像采集与传输系统需求文档

## 1. 功能需求

### 1.1 图像采集

- ​**​ 高清图像模式 ​**​：

  - 触发周期：5 秒/次
  - 分辨率：640x480 RGB
  - 处理流程：JPEG 压缩（质量 80%）→ 蓝牙传输
  - 优先级：当与低清模式冲突时优先执行, 即：在拍摄高清图像时，不拍摄低清图像

- ​**​ 低清图像模式 ​**​：
  - 触发频率：4 fps（持续运行）
  - 分辨率：128x128 灰度
  - 处理流程：
    1. 调用`cv2.calcOpticalFlowPyrLK`
    2. 采样 16x16 网格（共 256 点）
    3. 生成 512 维光流数据（x,y 向量各 256）
    4. 蓝牙传输光流数据

### 1.2 蓝牙传输

- 协议：经典蓝牙（BR/EDR）
- 传输内容：
  - 高清模式：JPEG 二进制流
  - 低清模式：512 字节光流数据（float32）

## 2. 功耗优化要求

- 非采集时段进入低功耗模式
- 自动切换分辨率（避免重复初始化）

# Hardware Schematic (ESP32 & 0v2640 camera)

| 字段名         | 描述                        | ESP32 对应引脚         |
| -------------- | --------------------------- | ---------------------- |
| `pin_pwdn`     | 摄像头电源控制引脚          | GPIO32                 |
| `pin_reset`    | 摄像头复位引脚              | NC                     |
| `pin_xclk`     | 摄像头 XCLK 引脚            | GPIO0                  |
| `pin_sccb_sda` | 摄像头 SDA 数据线(I2C)      | 对应 TWI_SDA(GPIO26)   |
| `pin_sccb_scl` | 摄像头 SCL 时钟线(I2C)      | 对应 TWI_SCK(GPIO27)   |
| `pin_d7`       | 摄像头数据线 D7             | 对应 CSI_D7(GPIO35)    |
| `pin_d6`       | 摄像头数据线 D6             | 对应 CSI_D6(GPIO34)    |
| `pin_d5`       | 摄像头数据线 D5             | 对应 CSI_D5(GPIO39)    |
| `pin_d4`       | 摄像头数据线 D4             | 对应 CSI_D4(GPIO36)    |
| `pin_d3`       | 摄像头数据线 D3             | 对应 CSI_D3(GPIO21)    |
| `pin_d2`       | 摄像头数据线 D2             | 对应 CSI_D2(GPIO19)    |
| `pin_d1`       | 摄像头数据线 D1             | 对应 CSI_D1(GPIO18)    |
| `pin_d0`       | 摄像头数据线 D0             | 对应 CSI_D0(GPIO5)     |
| `pin_vsync`    | 摄像头 VSYNC 垂直同步信号线 | 对应 CSI_VSYNC(GPIO25) |
| `pin_href`     | 摄像头 HREF 水平参考信号线  | 对应 CSI_HSYNC(GPIO23) |
| `pin_pclk`     | 摄像头 PCLK 像素时钟线      | 对应 CSI_PCLK(GPIO22)  |
