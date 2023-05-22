# Embeddede
该项目是一些关于嵌入式开发相关的例子
# 如何编译
1. 在example目录下找到你需要编译的例子
2. 执行以下命令进行编译
   ```shell
   make BUILD_DIR=uart_dma     # 以编译uart_dma例子为例
   ```
# 项目中已有的例子
| Example     | Platform  | Introduction |
| :---:       | :---:     | :---: |
| hello_world | x86       | [例程模板](https://github.com/luck-jay/embeddede/tree/master/example/hello_world) |
| uart_dma    | STM32F10x | [dma串口不定长大数据接收](https://github.com/luck-jay/embeddede/tree/master/example/uart_dma) |
| time_wheel  | STM32F10x | [软件定时器(基于时间轮)](https://github.com/luck-jay/embeddede/tree/master/example/time_wheel) |

# 如何做贡献
可以参考hello world例程，添加新的例程