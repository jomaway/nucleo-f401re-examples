# nucleo-f401re-examples
Collection of simple examples for the nucleo-f401re board used in my mcr lessons.

The repo is structured after the framework which is used. Some examples may exist in several versions.

- [Bare-metal](./Bare-metal/) uses raw register adresses and no framework at all. There are only two examples at the moment.
- [CMSIS](./CMSIS) uses the ARM CMSIS Framework. 
- [MCAL](./MCAL) uses a custom microcontroller abstraction library based on the CMSIS framework.
- [STM32Cube](./STM32Cube) uses the official STM32Cube HAL. There is only one example project at the moment. 

Plans are to add some `libopemcm3` examples in the future. 
 
## Setup

To test the examples you need to install **VSCode** and **PlatformIO**.

Each example is a complete platformIO project, which you can open directly with PlatformIO.
