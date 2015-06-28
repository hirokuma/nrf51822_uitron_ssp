/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 *
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  システムサービスのターゲット依存部
 *
 *  システムサービスのターゲット依存部のインクルードファイル．このファ
 *  イルの内容は，コンポーネント記述ファイルに記述され，このファイルは
 *  無くなる見込み．
 */

#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "nordic_nrf51822.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_TRACE_ENABLE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_TRACE_ENABLE */

/*
 *  起動メッセージのターゲットシステム名
 */
#define TARGET_NAME    "nRF51822"

/*
 *  起動メッセージにターゲット依存部の著作権表示を
 *  追加するためのマクロ．
 */
#ifdef PRC_COPYRIGHT
#define TARGET_COPYRIGHT	PRC_COPYRIGHT
#endif /* PRC_COPYRIGHT */

/*
 *  システムログの低レベル出力のための文字出力
 *
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．
 */
extern void	target_fput_log(char c);

/*
 *  シリアルポート数の定義
 *
 *  低レベルドライバ(PDIC)では USART0（ポート番号1）と USART1（ポート番号2）をサポートする
 *  シリアル・インタフェースドライバ(GDIC)では1ポートのみサポートする
 *
 */
#define TNUM_PORT        (1)	/* GDICでサポートするシリアルポートの数 */
#define TNUM_SIOP        (2)	/* PDICでサポートするシリアルポートの数 */
#if TNUM_PORT > TNUM_SIOP
#error "It must be TNUM_PORT <= TNUM_SIOP"
#endif


#ifndef TOPPERS_OMIT_SYSLOG
/*
 *  ボーレート
 *  動作確認済：230400, 115200, 57600, 38400, 28800, 19200, 14400,
 *            9600, 7200, 4800, 3600, 2400, 1800, 1200, 600, 300
 */
#define UART_BAUDRATE		(19200)

/*
 *  使用するシリアルポートID(低レベルドライバのID)
 */
#define SIO_PORTID		(1)

#endif	//TOPPERS_OMIT_SYSLOG

#endif /* TOPPERS_TARGET_SYSSVC_H */
