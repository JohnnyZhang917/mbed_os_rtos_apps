/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "mbed.h"
#include "platform/mbed_thread.h"


// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

void _SYS_FS_Tasks( void *pvParameters )
{
    while(1)
    {
        SYS_FS_Tasks();
        thread_sleep_for((uint32_t)(10 / MBED_OS_TICK_PERIOD_MS));
    }
}


static void _APP_Tasks(void *parameters)
{
    while(1)
    {
        APP_Tasks();
        thread_sleep_for((uint32_t)(5 / (MBED_OS_TICK_PERIOD_MS)));
    }
}


void _DRV_SDMMC_0_Tasks( void *pvParameters )
{
    while(1)
    {
        DRV_SDMMC_Tasks(sysObj.drvSDMMC0);
        thread_sleep_for((uint32_t)(DRV_SDMMC_RTOS_DELAY_IDX0 / MBED_OS_TICK_PERIOD_MS));
    }
}




void mbed_start(void)
{
    /* Create and Initialize a Singleton Mutex object */
    mbed_rtos_init_singleton_mutex();

    /* Maintain system services */
    
    Thread SYS_FS_thread((osPriority)(osPriorityNormal + (SYS_FS_PRIORITY - 1)), SYS_FS_STACK_SIZE, NULL, "_SYS_FS_Tasks");
    SYS_FS_thread.start(callback(_SYS_FS_Tasks, (void *)NULL));

    Thread DRV_SDMMC0_thread((osPriority)(osPriorityNormal + (DRV_SDMMC_PRIORITY_IDX0 - 1)), DRV_SDMMC_STACK_SIZE_IDX0, NULL, "_DRV_SDMMC0_Tasks");
    DRV_SDMMC0_thread.start(callback(_DRV_SDMMC_0_Tasks, (void *)NULL));





    /* Maintain Device Drivers */
    

    /* Maintain Middleware & Other Libraries */
    

    /* Maintain the application's state machine. */
        /* Create the _APP_Tasks task */
    Thread app_thread((osPriority)(osPriorityNormal + (1 - 1)), 4096, NULL, "_APP_Tasks");
    app_thread.start(callback(_APP_Tasks, (void *)0));




    while(1)
    {
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Initialize Mbed RTOS */
    mbed_rtos_init();
    /* Start Mbed RTOS */
    mbed_rtos_start();


}

/*******************************************************************************
 End of File
 */

