/*******************************************************************************
* File Name: op1.c
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

#include "op1.h"

uint8 op1_initVar = 0u; /* Defines if component was initialized */
static uint32 op1_internalPower = 0u; /* Defines component Power value */


/*******************************************************************************
* Function Name: op1_Init
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
void op1_Init(void)
{
    op1_internalPower = op1_POWER;
    op1_CTB_CTRL_REG = op1_DEFAULT_CTB_CTRL;
    op1_OA_RES_CTRL_REG = op1_DEFAULT_OA_RES_CTRL;
    op1_OA_COMP_TRIM_REG = op1_DEFAULT_OA_COMP_TRIM_REG;
}


/*******************************************************************************
* Function Name: op1_Enable
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
void op1_Enable(void)
{
    op1_OA_RES_CTRL_REG |= op1_internalPower | \
                                        op1_OA_PUMP_EN;
}


/*******************************************************************************
* Function Name: op1_Start
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
*  op1_initVar: Used to check the initial configuration, modified
*  when this function is called for the first time.
*
*******************************************************************************/
void op1_Start(void)
{
    if( 0u == op1_initVar)
    {
        op1_Init();
        op1_initVar = 1u;
    }
    op1_Enable();
}


/*******************************************************************************
* Function Name: op1_Stop
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
void op1_Stop(void)
{
    op1_OA_RES_CTRL_REG &= ((uint32)~(op1_OA_PWR_MODE_MASK | \
                                                   op1_OA_PUMP_EN));
}


/*******************************************************************************
* Function Name: op1_SetPower
********************************************************************************
*
* Summary:
*  Sets the Opamp to one of the three power levels.
*
* Parameters:
*  power: power levels.
*   op1_LOW_POWER - Lowest active power
*   op1_MED_POWER - Medium power
*   op1_HIGH_POWER - Highest active power
*
* Return:
*  None
*
**********************************************************************************/
void op1_SetPower(uint32 power)
{
    uint32 tmp;
    
    op1_internalPower = op1_GET_OA_PWR_MODE(power);
    tmp = op1_OA_RES_CTRL_REG & \
           (uint32)~op1_OA_PWR_MODE_MASK;
    op1_OA_RES_CTRL_REG = tmp | op1_internalPower;
}


/*******************************************************************************
* Function Name: op1_PumpControl
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
*   op1_PUMP_OFF - Turn off the pump
*   op1_PUMP_ON - Turn on the pump
*
* Return:
*  None
*
**********************************************************************************/
void op1_PumpControl(uint32 onOff)
{
    
    if(0u != onOff)
    {
        op1_OA_RES_CTRL |= op1_OA_PUMP_EN;    
    }
    else
    {
        op1_OA_RES_CTRL &= (uint32)~op1_OA_PUMP_EN;
    }
}


/* [] END OF FILE */
