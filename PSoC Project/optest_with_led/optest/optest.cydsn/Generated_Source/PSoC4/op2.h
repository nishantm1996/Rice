/*******************************************************************************
* File Name: op2.h
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


#if !defined(CY_OPAMP_op2_H)
#define CY_OPAMP_op2_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*       Type Definitions
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} op2_BACKUP_STRUCT;


/**************************************
*        Function Prototypes
**************************************/
void op2_Init(void);
void op2_Enable(void);
void op2_Start(void);
void op2_Stop(void);
void op2_SetPower(uint32 power);
void op2_PumpControl(uint32 onOff);
void op2_Sleep(void);
void op2_Wakeup(void);
void op2_SaveConfig(void);
void op2_RestoreConfig(void);


/**************************************
*           API Constants
**************************************/

/* Parameters for SetPower() function */
#define op2_LOW_POWER      (1u)
#define op2_MED_POWER      (2u)
#define op2_HIGH_POWER     (3u)


/* Parameters for PumpControl() function */
#define op2_PUMP_ON        (1u)
#define op2_PUMP_OFF       (0u)


/***************************************
*   Initial Parameter Constants
****************************************/

#define op2_OUTPUT_CURRENT         (1u)
#define op2_POWER                  (2u)
#define op2_MODE                   (1u)
#define op2_OA_COMP_TRIM_VALUE     (3u)
#define op2_DEEPSLEEP_SUPPORT      (0u)


/***************************************
*    Variables with External Linkage
***************************************/

extern uint8  op2_initVar;


/**************************************
*             Registers
**************************************/

#ifdef CYIPBLOCK_m0s8pass4b_VERSION
    #define op2_CTB_CTRL_REG       (*(reg32 *) op2_cy_psoc4_abuf__CTB_CTB_CTRL)
    #define op2_CTB_CTRL_PTR       ( (reg32 *) op2_cy_psoc4_abuf__CTB_CTB_CTRL)
#else
    #define op2_CTB_CTRL_REG       (*(reg32 *) op2_cy_psoc4_abuf__CTBM_CTB_CTRL)
    #define op2_CTB_CTRL_PTR       ( (reg32 *) op2_cy_psoc4_abuf__CTBM_CTB_CTRL)
#endif /* CYIPBLOCK_m0s8pass4b_VERSION */

#define op2_OA_RES_CTRL_REG    (*(reg32 *) op2_cy_psoc4_abuf__OA_RES_CTRL)
#define op2_OA_RES_CTRL_PTR    ( (reg32 *) op2_cy_psoc4_abuf__OA_RES_CTRL)
#define op2_OA_COMP_TRIM_REG   (*(reg32 *) op2_cy_psoc4_abuf__OA_COMP_TRIM)
#define op2_OA_COMP_TRIM_PTR   ( (reg32 *) op2_cy_psoc4_abuf__OA_COMP_TRIM)


/***************************************
*        Registers Constants
***************************************/

/* op2_CTB_CTRL_REG */
#define op2_CTB_CTRL_DEEPSLEEP_ON_SHIFT    (30u)   /* [30] Selects behavior CTB IP in the DeepSleep power mode */
#define op2_CTB_CTRL_ENABLED_SHIFT         (31u)   /* [31] Enable of the CTB IP */


#define op2_CTB_CTRL_DEEPSLEEP_ON          ((uint32) 0x01u << op2_CTB_CTRL_DEEPSLEEP_ON_SHIFT)
#define op2_CTB_CTRL_ENABLED               ((uint32) 0x01u << op2_CTB_CTRL_ENABLED_SHIFT)


/* op2_OA_RES_CTRL_REG */
#define op2_OA_PWR_MODE_SHIFT          (0u)    /* [1:0]    Power level */
#define op2_OA_DRIVE_STR_SEL_SHIFT     (2u)    /* [2]      Opamp output strenght select: 0 - 1x, 1 - 10x */
#define op2_OA_COMP_EN_SHIFT           (4u)    /* [4]      CTB IP mode: 0 - Opamp, 1 - Comparator  */
#define op2_OA_PUMP_EN_SHIFT           (11u)   /* [11]     Pump enable */


#define op2_OA_PWR_MODE                ((uint32) 0x02u << op2_OA_PWR_MODE_SHIFT)
#define op2_OA_PWR_MODE_MASK           ((uint32) 0x03u << op2_OA_PWR_MODE_SHIFT)
#define op2_OA_DRIVE_STR_SEL_1X        ((uint32) 0x00u << op2_OA_DRIVE_STR_SEL_SHIFT)
#define op2_OA_DRIVE_STR_SEL_10X       ((uint32) 0x01u << op2_OA_DRIVE_STR_SEL_SHIFT)
#define op2_OA_DRIVE_STR_SEL_MASK      ((uint32) 0x01u << op2_OA_DRIVE_STR_SEL_SHIFT)
#define op2_OA_COMP_EN                 ((uint32) 0x00u << op2_OA_COMP_EN_SHIFT)
#define op2_OA_PUMP_EN                 ((uint32) 0x01u << op2_OA_PUMP_EN_SHIFT)


/***************************************
*       Init Macros Definitions
***************************************/

#define op2_GET_DEEPSLEEP_ON(deepSleep)    ((0u != (deepSleep)) ? (op2_CTB_CTRL_DEEPSLEEP_ON) : (0u))
#define op2_GET_OA_DRIVE_STR(current)      ((0u != (current)) ? (op2_OA_DRIVE_STR_SEL_10X) : \
                                                                             (op2_OA_DRIVE_STR_SEL_1X))
#define op2_GET_OA_PWR_MODE(mode)          ((mode) & op2_OA_PWR_MODE_MASK)
#define op2_CHECK_PWR_MODE_OFF             (0u != (op2_OA_RES_CTRL_REG & \
                                                                op2_OA_PWR_MODE_MASK))

/* Returns true if component available in Deep Sleep power mode*/ 
#define op2_CHECK_DEEPSLEEP_SUPPORT        (0u != op2_DEEPSLEEP_SUPPORT) 

#define op2_DEFAULT_CTB_CTRL (op2_GET_DEEPSLEEP_ON(op2_DEEPSLEEP_SUPPORT) | \
                                           op2_CTB_CTRL_ENABLED)

#define op2_DEFAULT_OA_RES_CTRL (op2_OA_COMP_EN | \
                                              op2_GET_OA_DRIVE_STR(op2_OUTPUT_CURRENT))

#define op2_DEFAULT_OA_COMP_TRIM_REG (op2_OA_COMP_TRIM_VALUE)


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define op2_LOWPOWER                   (op2_LOW_POWER)
#define op2_MEDPOWER                   (op2_MED_POWER)
#define op2_HIGHPOWER                  (op2_HIGH_POWER)

/* PUMP ON/OFF defines */
#define op2_PUMPON                     (op2_PUMP_ON)
#define op2_PUMPOFF                    (op2_PUMP_OFF)

#define op2_OA_CTRL                    (op2_CTB_CTRL_REG)
#define op2_OA_RES_CTRL                (op2_OA_RES_CTRL_REG)

/* Bit Field  OA_CTRL */
#define op2_OA_CTB_EN_SHIFT            (op2_CTB_CTRL_ENABLED_SHIFT)
#define op2_OA_PUMP_CTRL_SHIFT         (op2_OA_PUMP_EN_SHIFT)
#define op2_OA_PUMP_EN_MASK            (0x800u)
#define op2_PUMP_PROTECT_MASK          (1u)


#endif    /* CY_OPAMP_op2_H */


/* [] END OF FILE */
