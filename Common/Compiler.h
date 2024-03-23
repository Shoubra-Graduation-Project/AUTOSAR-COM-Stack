 /******************************************************************************
 *
 * Module: Common - Compiler Abstraction
 *
 * File Name: Compiler.h
 *
 * Description:  This file contains the definitions and macros specified by
 *               AUTOSAR for the abstraction of compiler specific keywords.
 *
 * Author:
 *
 *******************************************************************************/

#ifndef COMPILER_H
#define COMPILER_H


#include "Std_Types.h"

#define _TEXAS_INSTRUMENTS_C_TM4C123_
#define _TEXAS_INSTRUMENTS_C_ARM_

/* Id for the company in the AUTOSAR
 * for example Mariam's ID = 1000 :) */
#define COMPILER_VENDOR_ID                                  (1000U)

/*
 * Module Version 1.0.0
 */
#define COMPILER_SW_MAJOR_VERSION                           (1U)
#define COMPILER_SW_MINOR_VERSION                           (0U)
#define COMPILER_SW_PATCH_VERSION                           (0U)

/*
 * AUTOSAR Version 4.3.1
 */
#define COMPILER_AR_RELEASE_MAJOR_VERSION                   (4U)
#define COMPILER_AR_RELEASE_MINOR_VERSION                   (3U)
#define COMPILER_AR_RELEASE_PATCH_VERSION                   (1U)

/*****************************************************************************/
/* Define:       AUTOMATIC                                                   */
/* Description:  Used for the declaration of local pointers                  */
/*****************************************************************************/
#define AUTOMATIC

/*****************************************************************************/
/* Define:       TYPEDEF                                                     */
/* Description:  The memory class shall be used within type definitions,     */
/*               where no memory qualifier can be specified                  */
/*****************************************************************************/
#define TYPEDEF

/*****************************************************************************/
/* Define:       NULL_PTR                                                    */
/* Description:  Void pointer to 0                                           */
/*****************************************************************************/
#define NULL_PTR ((void *)0x0)

/*****************************************************************************/
/* Define:       INLINE                                                      */
/* Description:  Abstraction of the keyword inline                           */
/*****************************************************************************/
#define INLINE __inline

/*****************************************************************************/
/* Define:       LOCAL_INLINE                                                */
/* Description:  Abstraction of keyword inline with static scope.            */
/*****************************************************************************/
#define LOCAL_INLINE static inline


/*****************************************************************************/
/* Define:       LOCAL 		                                                 */
/* Description:  Abstraction of keyword inline with static scope.            */
/*****************************************************************************/
#define LOCAL static

/*****************************************************************************/
/* Macro name: P2VAR                                                         */
/* Parameters: ptrtype     type of the referenced variable                   */
/*             memclass    classification of the pointer's variable itself   */
/*             ptrclass    defines the classification of the pointer's       */
/*                         distance                                          */
/*****************************************************************************/
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/*****************************************************************************/
/* Macro name: FUNC                                                          */
/* Parameters: rettype     return type of the function                       */
/*             memclass    classification of the function itself             */
/*****************************************************************************/
#define FUNC(rettype, memclass) rettype

/*****************************************************************************/
/* Macro name: P2VAR                                                         */
/* Parameters: ptrtype     type of the referenced variable                   */
/*             memclass    classification of the pointer's variable itself   */
/*             ptrclass    defines the classification of the pointer's       */
/*                         distance                                          */
/*****************************************************************************/
#define FUNC_P2VAR(ptrtype, memclass, ptrclass) ptrtype *


/*****************************************************************************/
/* Macro name: P2CONST                                                       */
/* Parameters: ptrtype     type of the referenced constant                   */
/*             memclass    classification of the pointer's varaible itself   */
/*             ptrclass    defines the classification of the pointer's       */
/*                         distance                                          */
/*****************************************************************************/
#define FUNC_P2CONST(ptrtype, memclass, ptrclass) const ptrtype *

/*****************************************************************************/
/* Macro name: CONSTP2VAR                                                    */
/* Parameters: ptrtype     type of the referenced variable                   */
/*             memclass    classification of the pointer's constant itself   */
/*             ptrclass    defines the classification of the pointer's       */
/*                         distance                                          */
/*****************************************************************************/
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const



/*****************************************************************************/
/* Macro name: CONSTP2CONST                                                  */
/* Parameters: ptrtype     type of the referenced constant                   */
/*             memclass    classification of the pointer's constant itself   */
/*             ptrclass    defines the classification of the pointer's       */
/*                         distance                                          */
/*****************************************************************************/
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

/*****************************************************************************/
/* Macro name: P2FUNC                                                        */
/* Parameters: rettype     return type of the function                       */
/*             ptrclass    defines the classification of the pointer's       */
/*                         distance                                          */
/*             fctname     function name respectively name of the defined    */
/*                         type                                              */
/*****************************************************************************/
#define P2FUNC(rettype, ptrclass, fctname) rettype (*fctname)

/*****************************************************************************/
/* Macro name: CONST                                                         */
/* Parameters: consttype   type of the constant                              */
/*             memclass    classification of the constant itself             */
/*****************************************************************************/
#define CONST(consttype, memclass) const consttype

/*****************************************************************************/
/* Macro name: VAR                                                           */
/* Parameters: vartype     type of the variable                              */
/*             memclass    classification of the variable itself             */
/*****************************************************************************/
#define VAR(vartype, memclass) vartype




#endif /* _COMPILER_H_ */
