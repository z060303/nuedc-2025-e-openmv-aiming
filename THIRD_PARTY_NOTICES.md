# 第三方组件

`firmware/Drivers/STM32F1xx_HAL_Driver` 是 STMicroelectronics 的 STM32F1 HAL 组件，目录内 `LICENSE.txt` 说明适用 BSD-3-Clause 许可（在无其他包级许可时）。

`firmware/Drivers/CMSIS/Include` 以及 `firmware/Drivers/CMSIS/Device/ST/STM32F1xx` 是 ARM/ST 提供的 CMSIS 组件；原目录中的 `LICENSE.txt` 已一同保留，分别说明 Apache-2.0 许可适用条件。

`firmware/Core/` 的 CubeMX 生成文件保留原有 ST 版权头。最初输入目录含有署名为 ZHANGDATOU 的 Emm V5 驱动示例代码，未找到其明确的再发布许可；本仓库没有发布那份示例，而是仅根据项目实际使用的 8 字节速度命令重新写了最小编码器。
