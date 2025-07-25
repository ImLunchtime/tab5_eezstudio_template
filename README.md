# EEZ Studio support for M5Tab5
# M5Tab5 EEZ Studio支持

本项目中包含了一个EEZ Studio项目（位于./main/Project/），可以通过EEZ Studio编辑该项目，并点击导出即可导出到ESP-IDF项目中，直接编译。

## 如何使用？
1. 用EEZ Studio打开./main/Project/项目文件
2. 在EEZ Studio中导出
3. 在根目录下执行idf.py build编译。编译完成后，执行idf.py flash烧录到Tab5设备中即可。