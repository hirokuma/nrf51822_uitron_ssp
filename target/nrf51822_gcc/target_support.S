/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2012 by Meika Sugimoto
 *  Copyright (C) 2013-2015 by Naoki Saito
 *             Nagoya Municipal Industrial Research Institute, JAPAN
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 */

/*
 * ターゲット依存部　アセンブリ言語
 */

#define TOPPERS_MACRO_ONLY
#define UINT_C(val)			(val)
#define ULONG_C(val)		(val)
#include "kernel_impl.h"
#include "nordic_nrf51822.h"

/*
 * 低レベルのターゲット依存の初期化
 *
 * メモリ初期化の前に呼び出される
 */
	.globl	hardware_init_hook


    .equ    NRF_POWER_RAMON_ADDRESS,             0x40000524
    .equ    NRF_POWER_RAMONB_ADDRESS,            0x40000554
    .equ    NRF_POWER_RAMONx_RAMxON_ONMODE_Msk,  0x3

	.text
	.align 2
	.syntax unified
	.code 16
	.thumb
	.type hardware_init_hook, function
hardware_init_hook:
/* Make sure ALL RAM banks are powered on */
    MOVS    R1, #NRF_POWER_RAMONx_RAMxON_ONMODE_Msk
    
    LDR     R0, =NRF_POWER_RAMON_ADDRESS
    LDR     R2, [R0]
    ORRS    R2, R1
    STR     R2, [R0]

    LDR     R0, =NRF_POWER_RAMONB_ADDRESS
    LDR     R2, [R0]
    ORRS    R2, R1
    STR     R2, [R0]


	bx	lr

