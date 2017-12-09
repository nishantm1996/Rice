/*******************************************************************************
* File Name: op2.c
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

#include "op2.h"

uint8 op2_initVar = 0u; /* Defines if component was initialized */
static uint32 op2_internalPower = 0u; /* Defines component Power value */


/*******************************************************************************
* Function Name: op2_Init
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
void op2_Init(void)
{
    op2_internalPower = op2_POWER;
    op2_CTB_CTRL_REG = op2_DEFAULT_CTB_CTRL;
    op2_OA_RES_CTRL_REG = op2_DEFAULT_OA_RES_CTRL;
    op2_OA_COMP_TRIM_REG = op2_DEFAULT_OA_COMP_TRIM_REG;
}


/*******************************************************************************
* Function Name: op2_Enable
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
void op2_Enable(void)
{
    op2_OA_RES_CTRL_REG |= op2_internalPower | \
                                        op2_OA_PUMP_EN;
}


/*******************************************************************************
* Function Name: op2_Start
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
*  op2_initVar: Used to check the initial configuration, modified
*  when this function is called for the first time.
*
*******************************************************************************/
void op2_Start(void)
{
    if( 0u == op2_initVar)
    {
        op2_Init();
        op2_initVar = 1u;
    }
    op2_Enable();
}


/*******************************************************************************
* Function Name: op2_Stop
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
void op2_Stop(void)
{
    op2_OA_RES_CTRL_REG &= ((uint32)~(op2_OA_PWR_MODE_MASK | \
                                                   op2_OA_PUMP_EN));
}


/*******************************************************************************
* Function Name: op2_SetPower
********************************************************************************
*
* Summary:
*  Sets the Opamp to one of the three power levels.
*
* Parameters:
*  power: power levels.
*   op2_LOW_POWER - Lowest active power
*   op2_MED_POWER - Medium power
*   op2_HIGH_POWER - Highest active power
*
* Return:
*  None
*
**********************************************************************************/
void op2_SetPower(uint32 power)
{
    uint32 tmp;
    
    op2_internalPower = op2_GET_OA_PWR_MODE(power);
    tmp = op2_OA_RES_CTRL_REG & \
           (uint32)~op2_OA_PWR_MODE_MASK;
    op2_OA_RES_CTRL_REG = tmp | op2_internalPower;
}


/*******************************************************************************
* Function Name: op2_PumpControl
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
*   op2_PUMP_OFF - Turn off the pump
*   op2_PUMP_ON - Turn on the pump
*
* Return:
*  None
*
**********************************************************************************/
void op2_PumpControl(uint32 onOff)
{
    
    if(0u != onOff)
    {
        op2_OA_RES_CTRL |= op2_OA_PUMP_EN;    
    }
    else
    {
        op2_OA_RES_CTRL &= (uint32)~op2_OA_PUMP_EN;
    }
}


/* [] END OF FILE */
