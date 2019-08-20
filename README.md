# STM LoRaWAN setup

This is a project that can send LoRaWAN messages.  
You can open it in STM32CubeIDE (version 1.0.2).  
The current setup is made for the NUCLEO-STM32L152 with an SX1276MB1MAS shield.

The library can be found in the [./LoRa/LoRaWAN](./LoRa/LoRaWAN) folder together with the hardware configuration in [./LoRa/Conf](./LoRa/Conf), Board Support Packages in [./LoRa/BSP](./LoRa/BSP) and a small intermediary interface to make using the library a little bit simpler in [./LoRa/LoRa.h](./LoRa/LoRa.h). Configuration of the keys and how joining is performed, is done in [./LoRa/Commissioning.h](./LoRa/Commissioning.h)

The project is configured with FreeRTOS, but this is not required. A task is created that will call a function in [./Core/Src/LoRaTask.c](./Core/Src/LoRaTask.c). That file has a little example on how to deal with the interface.

The project is set up to use the EU868 region. You can change it in the compiler definitions. Possible other values can be found in [.\LoRa\LoRaWAN\Mac\region\Region.h](.\LoRa\LoRaWAN\Mac\region\Region.h)

Keep in mind that when porting to a new project, that some interrupts have to call certain functions. You can find those interrupts in [.\Core\Src\stm32l1xx_it.c](.\Core\Src\stm32l1xx_it.c). (These are the RTC and EXTI# interrupts)  
There are also include paths you need to copy and you want to exclude the BSP files that aren't relevant for your hardware.

If you have questions or would like to see a change, then contact me personally or open an issue. But note that I'm not planning on fully maintaining this as it's just an example project.
