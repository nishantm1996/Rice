/*******************************************************************************
* File Name: op1.h
* Version 1.20
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Opamp (Analog Buffer) Component.
*
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_OPAMP_op1_H)
#define CY_OPAMP_op1_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*       Type Definitions
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} op1_BACKUP_STRUCT;


/**************************************
*        Function Prototypes
**************************************/
void op1_Init(void);
void op1_Enable(void);
void op1_Start(void);
void op1_Stop(void);
void op1_SetPower(uint32 power);
void op1_PumpControl(uint32 onOff);
void op1_Sleep(void);
void op1_Wakeup(void);
void op1_SaveConfig(void);
void op1_RestoreConfig(void);


/**************************************
*           API Constants
**************************************/

/* Parameters for SetPower() function */
#define op1_LOW_POWER      (1u)
#define op1_MED_POWER      (2u)
#define op1_HIGH_POWER     (3u)


/* Parameters for PumpControl() function */
#define op1_PUMP_ON        (1u)
#define op1_PUMP_OFF       (0u)


/***************************************
*   Initial Parameter Constants
****************************************/

#define op1_OUTPUT_CURRENT         (1u)
#define op1_POWER                  (2u)
#define op1_MODE                   (1u)
#define op1_OA_COMP_TRIM_VALUE     (3u)
#define op1_DEEPSLEEP_SUPPORT      (0u)


/***************************************
*    Variables with External Linkage
***************************************/

extern uint8  op1_initVar;


/**************************************
*             Registers
**************************************/

#ifdef CYIPBLOCK_m0s8pass4b_VERSION
    #define op1_CTB_CTRL_REG       (*(reg32 *) op1_cy_psoc4_abuf__CTB_CTB_CTRL)
    #define op1_CTB_CTRL_PTR       ( (reg32 *) op1_cy_psoc4_abuf__CTB_CTB_CTRL)
#else
    #define op1_CTB_CTRL_REG       (*(reg32 *) op1_cy_psoc4_abuf__CTBM_CTB_CTRL)
    #define op1_CTB_CTRL_PTR       ( (reg32 *) op1_cy_psoc4_abuf__CTBM_CTB_CTRL)
#endif /* CYIPBLOCK_m0s8pass4b_VERSION */

#define op1_OA_RES_CTRL_REG    (*(reg32 *) op1_cy_psoc4_abuf__OA_RES_CTRL)
#define op1_OA_RES_CTRL_PTR    ( (reg32 *) op1_cy_psoc4_abuf__OA_RES_CTRL)
#define op1_OA_COMP_TRIM_REG   (*(reg32 *) op1_cy_psoc4_abuf__OA_COMP_TRIM)
#define op1_OA_COMP_TRIM_PTR   ( (reg32 *) op1_cy_psoc4_abuf__OA_COMP_TRIM)


/***************************************
*        Registers Constants
***************************************/

/* op1_CTB_CTRL_REG */
#define op1_CTB_CTRL_DEEPSLEEP_ON_SHIFT    (30u)   /* [30] Selects behavior CTB IP in the DeepSleep power mode */
#define op1_CTB_CTRL_ENABLED_SHIFT         (31u)   /* [31] Enable of the CTB IP */


#define op1_CTB_CTRL_DEEPSLEEP_ON          ((uint32) 0x01u << op1_CTB_CTRL_DEEPSLEEP_ON_SHIFT)
#define op1_CTB_CTRL_ENABLED               ((uint32) 0x01u << op1_CTB_CTRL_ENABLED_SHIFT)


/* op1_OA_RES_CTRL_REG */
#define op1_OA_PWR_MODE_SHIFT          (0u)    /* [1:0]    Power level */
#define op1_OA_DRIVE_STR_SEL_SHIFT     (2u)    /* [2]      Opamp output strenght select: 0 - 1x, 1 - 10x */
#define op1_OA_COMP_EN_SHIFT           (4u)    /* [4]      CTB IP mode: 0 - Opamp, 1 - Comparator  */
#define op1_OA_PUMP_EN_SHIFT           (11u)   /* [11]     Pump enable */


#define op1_OA_PWR_MODE                ((uint32) 0x02u << op1_OA_PWR_MODE_SHIFT)
#define op1_OA_PWR_MODE_MASK           ((uint32) 0x03u << op1_OA_PWR_MODE_SHIFT)
#define op1_OA_DRIVE_STR_SEL_1X        ((uint32) 0x00u << op1_OA_DRIVE_STR_SEL_SHIFT)
#define op1_OA_DRIVE_STR_SEL_10X       ((uint32) 0x01u << op1_OA_DRIVE_STR_SEL_SHIFT)
#define op1_OA_DRIVE_STR_SEL_MASK      ((uint32) 0x01u << op1_OA_DRIVE_STR_SEL_SHIFT)
#define op1_OA_COMP_EN                 ((uint32) 0x00u << op1_OA_COMP_EN_SHIFT)
#define op1_OA_PUMP_EN                 ((uint32) 0x01u << op1_OA_PUMP_EN_SHIFT)


/***************************************
*       Init Macros Definitions
***************************************/

#define op1_GET_DEEPSLEEP_ON(deepSleep)    ((0u != (deepSleep)) ? (op1_CTB_CTRL_DEEPSLEEP_ON) : (0u))
#define op1_GET_OA_DRIVE_STR(current)      ((0u != (current)) ? (op1_OA_DRIVE_STR_SEL_10X) : \
                                                                             (op1_OA_DRIVE_STR_SEL_1X))
#define op1_GET_OA_PWR_MODE(mode)          ((mode) & op1_OA_PWR_MODE_MASK)
#define op1_CHECK_PWR_MODE_OFF             (0u != (op1_OA_RES_CTRL_REG & \
                                                                op1_OA_PWR_MODE_MASK))

/* Returns true if component available in Deep Sleep power mode*/ 
#define op1_CHECK_DEEPSLEEP_SUPPORT        (0u != op1_DEEPSLEEP_SUPPORT) 

#define op1_DEFAULT_CTB_CTRL (op1_GET_DEEPSLEEP_ON(op1_DEEPSLEEP_SUPPORT) | \
                                           op1_CTB_CTRL_ENABLED)

#define op1_DEFAULT_OA_RES_CTRL (op1_OA_COMP_EN | \
                                              op1_GET_OA_DRIVE_STR(op1_OUTPUT_CURRENT))

#define op1_DEFAULT_OA_COMP_TRIM_REG (op1_OA_COMP_TRIM_VALUE)


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define op1_LOWPOWER                   (op1_LOW_POWER)
#define op1_MEDPOWER                   (op1_MED_POWER)
#define op1_HIGHPOWER                  (op1_HIGH_POWER)

/* PUMP ON/OFF defines */
#define op1_PUMPON                     (op1_PUMP_ON)
#define op1_PUMPOFF                    (op1_PUMP_OFF)

#define op1_OA_CTRL                    (op1_CTB_CTRL_REG)
#define op1_OA_RES_CTRL                (op1_OA_RES_CTRL_REG)

/* Bit Field  OA_CTRL */
#define op1_OA_CTB_EN_SHIFT            (op1_CTB_CTRL_ENABLED_SHIFT)
#define op1_OA_PUMP_CTRL_SHIFT         (op1_OA_PUMP_EN_SHIFT)
#define op1_OA_PUMP_EN_MASK            (0x800u)
#define op1_PUMP_PROTECT_MASK          (1u)


#endif    /* CY_OPAMP_op1_H */


/* [] END OF FILE */
