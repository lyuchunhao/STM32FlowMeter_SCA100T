#### 项目介绍
2020年与二师弟一块开发的唐山积算仪项目，其中关键角度模块传感器主要芯片为SCA100T，但由于缺少物料被替换SCL3300.

#### 更新日志
2022-01-12:
1. 采用第二版硬件
2. STM32L152X_FlowMeter_Hw2_V2.5.0作为Git初始版本
3. 调整了Rs485的使能端口PA8

#### 历史版本 
1. STM32L152X_FlowMeter_V1.0       
- a. Demo硬件版本(跳线)                                
- b. 只输出X轴-IC2
								   
2. STM32L152X_FlowMeter_V2.0       
- a. 2019-09-12吴信超第一次展示样机版本
- b. 只输出X轴-IC2
- c. 正式PCB版本
- d. 串口字符串输出
- e. 参考协议：<<FlowMeter协议>>

3. STM32L152X_FlowMeter_V2.1       
- a. X轴Y轴添加完毕                             
- b. XY电压值8次平均处理
- c. FIR滤波 + 20sps + AD精度20bit
- d. 添加零点设置+满值标定+温度输出
- e. 参考协议：<<FlowMeterV1.0>>
								   
4. STM32L152X_FlowMeter_V2.2       
- a. 添加EEPROM存储 (data: 2016-09-19)
- b. 解决串口丢包BUG
- c. 增加恢复出厂设置+配置参数查看
- d. 作为第二次验收版本
								   
5. STM32L152X_FlowMeter_V2.3       
- a. 采样速率调整为2400,最快0.15s/10° = 15ms/1°=>4*8*0.5ms
- b. 将RS232兼容为485,SCA1_ST1(PA2)为Rs485使能端1-发送 0-接收
- c. 2.4K sps 8次一级平均滤波
- d. 解决X/Y轴输出bug,原始改为零点和系数补偿后的数据输出
- e. SCA1_ST1作为Rs485的接收发送使能(跳线)
								   
6. STM32L152X_FlowMeter_V2.3.1     
- a. 在版本<<STM32L152X_FlowMeter_V2.3>>的基础上
- b. FIR滤波 20Hz(8次平滑滤波)
- c. X轴输出开关(默认关闭)
- d. SCA1_ST1作为Rs485的接收发送使能(跳线)
				
7. STM32L152X_FlowMeter_V2.4       
- a. 2级滤波(均8次)
- b. Sinc4 采样速率调整为2400
- c. SCA1_ST1作为Rs485的接收发送使能(跳线)
							   
8. STM32L152X_FlowMeter_V2.4.1     
- a. 2级滤波(均8次)
- b. FIR 采样速率调整为20Hz
- c. SCA1_ST1作为Rs485的接收发送使能(跳线)
	
9. STM32L152X_FlowMeter_Hw2_V2.5.0 
- a. 第二版硬件
- b. 在版本<<STM32L152X_FlowMeter_V2.3.1>>基础上修改
- c. 调整了Rs485的使能端口PA8
- d. 作为该硬件下的正式发布版本
								   
10.STM32L152X_FlowMeter_Hw2_V2.5.0 - TEST
- a. 吴两块板子晶振无法起振，但改为内部晶振就可以
- b. 提供的测试版本
