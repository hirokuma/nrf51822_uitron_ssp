/*
 * Copyright (c) 2012-2014, hiro99ma
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *         this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *         this list of conditions and the following disclaimer
 *         in the documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

/**************************************************************************
 * include
 **************************************************************************/

#include <kernel.h>
#include "kernel_cfg.h"
#include "app_task.h"

#include <string.h>

#include "boards.h"
#include "drivers.h"
#include "app_ble.h"

#include "app_error.h"
#include "app_trace.h"


/**************************************************************************
 * macro
 **************************************************************************/

/**************************************************************************
 * declaration
 **************************************************************************/

/**************************************************************************
 * prototype
 **************************************************************************/

/**************************************************************************
 * main entry
 **************************************************************************/

/**@brief main
 */
void app_task(intptr_t exinf)
{
    // 初期化
    drv_init();
    app_ble_init();

    app_trace_init();
    app_trace_log("START\r\n");

    // 処理開始
    //timers_start();
    app_ble_start();

    // メインループ
    while (1) {
        drv_event_exec();
    }
}


/**************************************************************************
 * private function
 **************************************************************************/

