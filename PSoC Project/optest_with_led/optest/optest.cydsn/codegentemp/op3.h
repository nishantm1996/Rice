/*******************************************************************************
* File Name: op3.h
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


#if !defined(CY_OPAMP_op3_H)
#define CY_OPAMP_op3_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*       Type Definitions
***************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} op3_BACKUP_STRUCT;


/**************************************
*        Function Prototypes
**************************************/
void op3_Init(void);
void op3_Enable(void);
void op3_Start(void);
void op3_Stop(void);
void op3_SetPower(uint32 power);
void op3_PumpControl(uint32 onOff);
void op3_Sleep(void);
void op3_Wakeup(void);
void op3_SaveConfig(void);
void op3_RestoreConfig(void);


/**************************************
*           API Constants
**************************************/

/* Parameters for SetPower() function */
#define op3_LOW_POWER      (1u)
#define op3_MED_POWER      (2u)
#define op3_HIGH_POWER     (3u)


/* Parameters for PumpControl() function */
#define op3_PUMP_ON        (1u)
#define op3_PUMP_OFF       (0u)


/***************************************
*   Initial Parameter Constants
****************************************/

#define op3_OUTPUT_CURRENT         (1u)
#define op3_POWER                  (2u)
#define op3_MODE                   (0u)
#define op3_OA_COMP_TRIM_VALUE     (3u)
#define op3_DEEPSLEEP_SUPPORT      (0u)


/***************************************
*    Variables with External Linkage
***************************************/

extern uint8  op3_initVar;


/**************************************
*             Registers
**************************************/

#ifdef CYIPBLOCK_m0s8pass4b_VERSION
    #define op3_CTB_CTRL_REG       (*(reg32 *) op3_cy_psoc4_abuf__CTB_CTB_CTRL)
    #define op3_CTB_CTRL_PTR       ( (reg32 *) op3_cy_psoc4_abuf__CTB_CTB_CTRL)
#else
    #define op3_CTB_CTRL_REG       (*(reg32 *) op3_cy_psoc4_abuf__CTBM_CTB_CTRL)
    #define op3_CTB_CTRL_PTR       ( (reg32 *) op3_cy_psoc4_abuf__CTBM_CTB_CTRL)
#endif /* CYIPBLOCK_m0s8pass4b_VERSION */

#define op3_OA_RES_CTRL_REG    (*(reg32 *) op3_cy_psoc4_abuf__OA_RES_CTRL)
#define op3_OA_RES_CTRL_PTR    ( (reg32 *) op3_cy_psoc4_abuf__OA_RES_CTRL)
#define op3_OA_COMP_TRIM_REG   (*(reg32 *) op3_cy_psoc4_abuf__OA_COMP_TRIM)
#define op3_OA_COMP_TRIM_PTR   ( (reg32 *) op3_cy_psoc4_abuf__OA_COMP_TRIM)


/***************************************
*        Registers Constants
***************************************/

/* op3_CTB_CTRL_REG */
#define op3_CTB_CTRL_DEEPSLEEP_ON_SHIFT    (30u)   /* [30] Selects behavior CTB IP in the DeepSleep power mode */
#define op3_CTB_CTRL_ENABLED_SHIFT         (31u)   /* [31] Enable of the CTB IP */


#define op3_CTB_CTRL_DEEPSLEEP_ON          ((uint32) 0x01u << op3_CTB_CTRL_DEEPSLEEP_ON_SHIFT)
#define op3_CTB_CTRL_ENABLED               ((uint32) 0x01u << op3_CTB_CTRL_ENABLED_SHIFT)


/* op3_OA_RES_CTRL_REG */
#define op3_OA_PWR_MODE_SHIFT          (0u)    /* [1:0]    Power level */
#define op3_OA_DRIVE_STR_SEL_SHIFT     (2u)    /* [2]      Opamp output strenght select: 0 - 1x, 1 - 10x */
#define op3_OA_COMP_EN_SHIFT           (4u)    /* [4]      CTB IP mode: 0 - Opamp, 1 - Comparator  */
#define op3_OA_PUMP_EN_SHIFT           (11u)   /* [11]     Pump enable */


#define op3_OA_PWR_MODE                ((uint32) 0x02u << op3_OA_PWR_MODE_SHIFT)
#define op3_OA_PWR_MODE_MASK           ((uint32) 0x03u << op3_OA_PWR_MODE_SHIFT)
#define op3_OA_DRIVE_STR_SEL_1X        ((uint32) 0x00u << op3_OA_DRIVE_STR_SEL_SHIFT)
#define op3_OA_DRIVE_STR_SEL_10X       ((uint32) 0x01u << op3_OA_DRIVE_STR_SEL_SHIFT)
#define op3_OA_DRIVE_STR_SEL_MASK      ((uint32) 0x01u << op3_OA_DRIVE_STR_SEL_SHIFT)
#define op3_OA_COMP_EN                 ((uint32) 0x00u << op3_OA_COMP_EN_SHIFT)
#define op3_OA_PUMP_EN                 ((uint32) 0x01u << op3_OA_PUMP_EN_SHIFT)


/***************************************
*       Init Macros Definitions
***************************************/

#define op3_GET_DEEPSLEEP_ON(deepSleep)    ((0u != (deepSleep)) ? (op3_CTB_CTRL_DEEPSLEEP_ON) : (0u))
#define op3_GET_OA_DRIVE_STR(current)      ((0u != (current)) ? (op3_OA_DRIVE_STR_SEL_10X) : \
                                                                             (op3_OA_DRIVE_STR_SEL_1X))
#define op3_GET_OA_PWR_MODE(mode)          ((mode) & op3_OA_PWR_MODE_MASK)
#define op3_CHECK_PWR_MODE_OFF             (0u != (op3_OA_RES_CTRL_REG & \
                                                                op3_OA_PWR_MODE_MASK))

/* Returns true if component available in Deep Sleep power mode*/ 
#define op3_CHECK_DEEPSLEEP_SUPPORT        (0u != op3_DEEPSLEEP_SUPPORT) 

#define op3_DEFAULT_CTB_CTRL (op3_GET_DEEPSLEEP_ON(op3_DEEPSLEEP_SUPPORT) | \
                                           op3_CTB_CTRL_ENABLED)

#define op3_DEFAULT_OA_RES_CTRL (op3_OA_COMP_EN | \
                                              op3_GET_OA_DRIVE_STR(op3_OUTPUT_CURRENT))

#define op3_DEFAULT_OA_COMP_TRIM_REG (op3_OA_COMP_TRIM_VALUE)


/***************************************
* The following code is DEPRECATED and 
* should not be used in new projects.
***************************************/

#define op3_LOWPOWER                   (op3_LOW_POWER)
#define op3_MEDPOWER                   (op3_MED_POWER)
#define op3_HIGHPOWER                  (op3_HIGH_POWER)

/* PUMP ON/OFF defines */
#define op3_PUMPON                     (op3_PUMP_ON)
#define op3_PUMPOFF                    (op3_PUMP_OFF)

#define op3_OA_CTRL                    (op3_CTB_CTRL_REG)
#define op3_OA_RES_CTRL                (op3_OA_RES_CTRL_REG)

/* Bit Field  OA_CTRL */
#define op3_OA_CTB_EN_SHIFT            (op3_CTB_CTRL_ENABLED_SHIFT)
#define op3_OA_PUMP_CTRL_SHIFT         (op3_OA_PUMP_EN_SHIFT)
#define op3_OA_PUMP_EN_MASK            (0x800u)
#define op3_PUMP_PROTECT_MASK          (1u)


#endif    /* CY_OPAMP_op3_H */


/* [] END OF FILE */
