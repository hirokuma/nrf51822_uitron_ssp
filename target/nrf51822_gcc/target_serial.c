/*
 *  TOPPERS/SSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
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
 * シリアルドライバ（nRF51822用）
 */

#include <kernel.h>
#include <sil.h>
//#include "kernel_int.h"
#include "target_serial.h"
#include "target_syssvc.h"

/*
 * レジスタ設定値
 */
#define INDEX2SIOPID(index) 	((index) + 1)	// インデックスからポートIDへ
#define SIOPID2INDEX(siopid)	((siopid) - 1)	// ポートIDからインデックスへ
#define GET_SIOPCB(siopid)  	(&siopcb_table[SIOPID2INDEX(siopid)])
#define GET_SIOPID(siopcb)  	((siopcb-(uint32_t)siopcb_table)/sizeof(siopcb_table[0]))

/*
 * シリアルI/Oポート管理ブロックエリア
 */
SIOPCB siopcb_table[TNUM_PORT];

// シリアルドライバ(GDIC)のポートIDに対応するシリアルポート(PDIC)のレジスタベースアドレス
// 対応関係を変更したい場合(例えばポートID1をUSART1に対応づける場合)，ここを変更する
static const uint32_t sioreg_table[TNUM_PORT] = {
	TADR_USART_BASE(0),
#if (TNUM_PORT == 2)
	TADR_USART_BASE(1),
#endif
};

Inline bool_t  sio_putready(SIOPCB* siopcb)
{
	uint32_t tmp;
	tmp = sil_rew_mem((void *)TADR_USART_STAT(siopcb->reg));

	return (tmp & TBITPTN_USART_STAT_TXRDY) != 0;
}

Inline bool_t sio_getready(SIOPCB* siopcb)
{
	uint32_t tmp;
	tmp = sil_rew_mem((void *)TADR_USART_STAT(siopcb->reg));

	return (tmp & TBITPTN_USART_STAT_RXRDY) != 0;
}

static bool_t initialized[TNUM_SIOP] = {false};
/*
 *  低レベル出力の初期化
 */
void target_low_output_init(ID siopid)
{
	uint32_t base = TADR_USART_BASE(SIOPID2INDEX(siopid));
	volatile uint32_t tmp;

	if(initialized[siopid]) {return;}

	// UART回路に供給するクロック（メインクロックの分周比）
	sil_wrw_mem((void *)TADR_SYSCON_UARTCLKDIV, TVALUE_SYSCON_UARTCLKDIV);

	/* USARTの無効化，リセット */
	dis_int(INT_UART0);
	tmp = sil_rew_mem((void *)TADR_USART_CFG(base));
	tmp &= ~TBITPTN_USART_CFG_ENABLE;
	sil_wrw_mem((void *)TADR_USART_CFG(base), tmp);

	// UART0のクロックをON
	tmp = sil_rew_mem((void *)TADR_SYSCON_SYSAHBCLKCTRL);
	tmp |= TBITPTN_SYSCON_SYSAHBCLKCTRL_UART0;
	sil_wrw_mem((void *)TADR_SYSCON_SYSAHBCLKCTRL, tmp);

	// デバイスをリセット
	tmp = sil_rew_mem((void *)TADR_SYSCON_PRESETCTRL);
	tmp &= ~TBITPTN_SYSCON_PRESETCTRL_USART0_RST_N;
	sil_wrw_mem((void *)TADR_SYSCON_PRESETCTRL, tmp);
	tmp |= TBITPTN_SYSCON_PRESETCTRL_USART0_RST_N;
	sil_wrw_mem((void *)TADR_SYSCON_PRESETCTRL, tmp);

	/* 通信モード設定 */
	tmp = (TBITPTN_USART_CFG_DATALEN_8BIT | TBITPTN_USART_CFG_PARITYSEL_NOPARITY | TBITPTN_USART_CFG_STOPLEN_1BIT);
	sil_wrw_mem((void *)TADR_USART_CFG(base), tmp);

	/* 通信速度設定 */
	sil_wrw_mem((void *)TADR_USART_BRG(base), TVALUE_USART_BRG(UART_BAUDRATE));
	sil_wrw_mem((void *)TADR_SYSCON_UARTFRGDIV, TVALUE_SYSCON_UARTFRGDIV);
	sil_wrw_mem((void *)TADR_SYSCON_UARTFRGMULT, TVALUE_SYSCON_UARTFRGMULT(UART_BAUDRATE));

	// CTS/RTSのクリア
	sil_wrw_mem((void *)TADR_USART_STAT(base), (TBITPTN_USART_STAT_DELTACTS | TBITPTN_USART_STAT_DELTARXBRK));

	// UARTの割込み許可
//	ena_int(INT_UART0);

	// UARTの個別の割込み有効/無効(受信：有効，送信：無効)
//	sil_wrw_mem((void *)TADR_USART_INTENSET(base), TBITPTN_USART_INTENSET_RXRDYEN);
//	sil_wrw_mem((void *)TADR_USART_INTENCLR(base), TBITPTN_USART_INTENCLR_TXRDYCLR);

	/* USARTの有効化 */
	tmp = sil_rew_mem((void *)TADR_USART_CFG(base));
	tmp |= TBITPTN_USART_CFG_ENABLE;
	sil_wrw_mem((void *)TADR_USART_CFG(base), tmp);

	initialized[siopid] = true;
}

/*
 *  ターゲットのシリアル初期化
 */
void target_usart_init(ID siopid)
{
	target_low_output_init(siopid);

	/* 割込み許可 */
	(void)ena_int(INTNO_SIO);
}

/*
 *  ターゲットのシリアル終了
 */
void target_usart_term(uint32_t base)
{
	uint32_t tmp;

	/* 割込み禁止 */
	(void)dis_int(INTNO_SIO);

	/* USARTの無効化 */
	tmp = sil_rew_mem((void *)TADR_USART_CFG(base));
	tmp &= ~TBITPTN_USART_CFG_ENABLE;
	sil_wrw_mem((void *)TADR_USART_CFG(base), tmp);

	int_t siopid = GET_SIOPID(base);
	initialized[siopid] = false;
}

/*
 *  SIO初期化
 */
void sio_initialize(intptr_t exinf)
{
	int i;

	for (i = 0; i < TNUM_PORT; i++) {
		siopcb_table[i].reg = sioreg_table[i];
		siopcb_table[i].exinf = 0;
	}
}

/*
 *  シリアルオープン
 */
SIOPCB *sio_opn_por(ID siopid, intptr_t exinf)
{
	SIOPCB* siopcb;

	if (siopid > TNUM_PORT || siopid < 0) {
		return NULL;
	}

	target_usart_init(siopid);

	siopcb = GET_SIOPCB(siopid);
	siopcb->exinf = exinf;

	return siopcb;
}

/*
 *  シリアルクローズ
 */
void sio_cls_por(SIOPCB *p_siopcb)
{
	target_usart_term(p_siopcb->reg);
}

/*
 *  割込みハンドラ
 */
void sio_isr(intptr_t exinf)
{
	SIOPCB* siopcb = GET_SIOPCB(exinf);

    if (sio_putready(siopcb)) {
            sio_irdy_snd(siopcb->exinf);
    }
    if (sio_getready(siopcb)) {
            sio_irdy_rcv(siopcb->exinf);
    }
}


/*
 *  1文字送信
 */
bool_t sio_snd_chr(SIOPCB *siopcb, char c)
{
	bool_t ret = false;

	if (sio_putready(siopcb)) {
		sil_wrw_mem((void *)TADR_USART_TXDATA(siopcb->reg), c);
		ret = true;
	}

	return ret;
}

/*
 *  1文字受信
 */
int_t sio_rcv_chr(SIOPCB *siopcb)
{
	int_t c = -1;

	if (sio_getready(siopcb)) {
		c = (int_t)sil_rew_mem((void *)TADR_USART_RXDATA(siopcb->reg));
	}

	return c;
}

/*
 *  コールバックの許可
 */
void sio_ena_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	case SIO_RDY_SND:
		sil_wrw_mem((void *)TADR_USART_INTENSET(siopcb->reg), TBITPTN_USART_INTENSET_TXRDYEN);
		break;
	case SIO_RDY_RCV:
		sil_wrw_mem((void *)TADR_USART_INTENSET(siopcb->reg), TBITPTN_USART_INTENSET_RXRDYEN);
		break;
	default:
		break;
	}
}

/*
 *  コールバックの禁止
 */
void sio_dis_cbr(SIOPCB *siopcb, uint_t cbrtn)
{
	switch (cbrtn) {
	case SIO_RDY_SND:
		sil_wrw_mem((void *)TADR_USART_INTENCLR(siopcb->reg), TBITPTN_USART_INTENCLR_TXRDYCLR);
		break;
	case SIO_RDY_RCV:
		sil_wrw_mem((void *)TADR_USART_INTENCLR(siopcb->reg), TBITPTN_USART_INTENCLR_RXRDYCLR);
		break;
	default:
		break;
	}
}

/*
 *  1文字出力（ポーリングでの出力）
 */
void sio_pol_snd_chr(char c, ID siopid)
{
	uint32_t base = TADR_USART_BASE(SIOPID2INDEX(siopid));
	volatile uint32_t tmp;

	do {
		tmp = sil_rew_mem((void *)TADR_USART_STAT(base));
	} while((tmp & TBITPTN_USART_STAT_TXRDY) == 0);

	sil_wrw_mem((void *)TADR_USART_TXDATA(base), (uint32_t)c);
}
