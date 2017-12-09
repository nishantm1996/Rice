/*******************************************************************************
* File Name: op1_PM.c
* Version 1.20
*
* Description:
*  This file provides the power management source code to the API for the
*  Opamp (Analog Buffer) component.
*
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "op1.h"

#if(!op1_CHECK_DEEPSLEEP_SUPPORT)
    static op1_BACKUP_STRUCT op1_backup =
    {
        0u, /* enableState */
    };
#endif /* (op1_CHECK_DEEPSLEEP_SUPPORT) */


/*******************************************************************************
* Function Name: op1_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void op1_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: op1_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void op1_RestoreConfig(void)
{
    
}


/*******************************************************************************
* Function Name: op1_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. The Sleep() API 
*  saves the current component state. Call the Sleep() function before calling the 
*  CySysPmDeepSleep() or the CySysPmHibernate() functions. The "Deep sleep operation" 
*  option has an influence on this function implementation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  op1_backup: The structure field 'enableState' is modified
*  depending on the enable state of the block before entering the sleep mode.
*
*******************************************************************************/
void op1_Sleep(void)
{
#if(!op1_CHECK_DEEPSLEEP_SUPPORT)
    if(op1_CHECK_PWR_MODE_OFF)
    {
        op1_backup.enableState = 1u;
        op1_Stop();
    }
    else /* The component is disabled */
    {
        op1_backup.enableState = 0u;
    }
#endif /* (op1_CHECK_DEEPSLEEP_SUPPORT) */
}


/*******************************************************************************
* Function Name: op1_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when Sleep() 
*  is called. If the component has been enabled before the Sleep() function is 
*  called, the Wakeup() function will also re-enable the component.
*  The "Deep sleep operation" option has an influence on this function
*  implementation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  op1_backup: The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void op1_Wakeup(void)
{
#if(!op1_CHECK_DEEPSLEEP_SUPPORT)
    if(0u != op1_backup.enableState)
    {
        /* Enable Opamp's operation */
        op1_Enable();
    } /* Do nothing if Opamp was disabled before */
#endif /* (op1_CHECK_DEEPSLEEP_SUPPORT) */
}


/* [] END OF FILE */
