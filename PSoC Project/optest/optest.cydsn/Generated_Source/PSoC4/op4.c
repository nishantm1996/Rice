/*******************************************************************************
* File Name: op4.c
* Version 1.20
*
* Description:
*  This file provides the source code to the API for the Opamp (Analog Buffer)
*  Component.
*
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "op4.h"

uint8 op4_initVar = 0u; /* Defines if component was initialized */
static uint32 op4_internalPower = 0u; /* Defines component Power value */


/*******************************************************************************
* Function Name: op4_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure 
*  dialog settings. It is not necessary to call Init() because the Start() API 
*  calls this function and is the preferred method to begin the component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void op4_Init(void)
{
    op4_internalPower = op4_POWER;
    op4_CTB_CTRL_REG = op4_DEFAULT_CTB_CTRL;
    op4_OA_RES_CTRL_REG = op4_DEFAULT_OA_RES_CTRL;
    op4_OA_COMP_TRIM_REG = op4_DEFAULT_OA_COMP_TRIM_REG;
}


/*******************************************************************************
* Function Name: op4_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins the component operation. It is not necessary to 
*  call Enable() because the Start() API calls this function, which is the 
*  preferred method to begin the component operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void op4_Enable(void)
{
    op4_OA_RES_CTRL_REG |= op4_internalPower | \
                                        op4_OA_PUMP_EN;
}


/*******************************************************************************
* Function Name: op4_Start
********************************************************************************
*
* Summary:
*  Performs all of the required initialization for the component and enables power 
*  to the block. The first time the routine is executed, the Power level, Mode, 
*  and Output mode are set. When called to restart the Opamp following a Stop() call, 
*  the current component parameter settings are retained.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  op4_initVar: Used to check the initial configuration, modified
*  when this function is called for the first time.
*
*******************************************************************************/
void op4_Start(void)
{
    if( 0u == op4_initVar)
    {
        op4_Init();
        op4_initVar = 1u;
    }
    op4_Enable();
}


/*******************************************************************************
* Function Name: op4_Stop
********************************************************************************
*
* Summary:
*  Turn off the Opamp block.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void op4_Stop(void)
{
    op4_OA_RES_CTRL_REG &= ((uint32)~(op4_OA_PWR_MODE_MASK | \
                                                   op4_OA_PUMP_EN));
}


/*******************************************************************************
* Function Name: op4_SetPower
********************************************************************************
*
* Summary:
*  Sets the Opamp to one of the three power levels.
*
* Parameters:
*  power: power levels.
*   op4_LOW_POWER - Lowest active power
*   op4_MED_POWER - Medium power
*   op4_HIGH_POWER - Highest active power
*
* Return:
*  None
*
**********************************************************************************/
void op4_SetPower(uint32 power)
{
    uint32 tmp;
    
    op4_internalPower = op4_GET_OA_PWR_MODE(power);
    tmp = op4_OA_RES_CTRL_REG & \
           (uint32)~op4_OA_PWR_MODE_MASK;
    op4_OA_RES_CTRL_REG = tmp | op4_internalPower;
}


/*******************************************************************************
* Function Name: op4_PumpControl
********************************************************************************
*
* Summary:
*  Allows the user to turn the Opamp's boost pump on or off. By Default the Start() 
*  function turns on the pump. Use this API to turn it off. The boost must be 
*  turned on when the supply is less than 2.7 volts and off if the supply is more 
*  than 4 volts.
*
* Parameters:
*  onOff: Control the pump.
*   op4_PUMP_OFF - Turn off the pump
*   op4_PUMP_ON - Turn on the pump
*
* Return:
*  None
*
**********************************************************************************/
void op4_PumpControl(uint32 onOff)
{
    
    if(0u != onOff)
    {
        op4_OA_RES_CTRL |= op4_OA_PUMP_EN;    
    }
    else
    {
        op4_OA_RES_CTRL &= (uint32)~op4_OA_PUMP_EN;
    }
}


/* [] END OF FILE */
