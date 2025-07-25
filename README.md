# EEZ Studio support for M5Tab5
# M5Tab5 EEZ Studio支持
由m5stack-tab5-lvgl修改5
本项目中包含了一个EEZ Studio项目（位于`根目录/main/Project/`），可以通过EEZ Studio编辑该项目，并点击导出即可导出到ESP-IDF项目中，直接编译。
Modified from m5stack-tab5-lvgl. This project contains an EEZ Studio project at `root/main/project`. You can edit the project in EEZ Studio and export it, the exported code will be automatically imported to the ESP-IDF project.

## 什么是EEZ Studio?
EEZ Studio是一款开源免费的，所见即所得的嵌入式设备界面设计工具。使用EEZ Studio可以低代码设计LVGL界面。相比于更专业且某些地方更方便的SquareLine Studio，EEZ Studio好在完全免费，且一些用户交互逻辑上更直观。
## What is EEZ Studio?
EEZ Studio is a FOSS, WYSIWYG embedded GUI designing tool. You can use EEZ Studio to create LVGL UIs. Compared to the more professional software SquareLine Studio, EEZ Studio is free and more user-friendly at some points.

## 如何使用？
1. 用EEZ Studio打开`根目录/main/Project/`项目文件
2. 编辑项目
3. 在EEZ Studio中导出
4. 在根目录下执行idf.py build编译。编译完成后，执行idf.py flash烧录到Tab5设备中即可。
## How to use it?
1. Open the EEZ Studio Project at `root/main/Project/Project.eez-project`
2. Edit the project
3. Export in EEZ Studio
4. Build using `idf.py build` and flash.