# TOPPERS/SSP + nRF51822
----

## base platform

https://dev.toppers.jp/trac_user/contrib/wiki/armv6_m_gcc

----

## build

$ mkdir build  
$ cd build  
$ ../configure -T nrf51822_gcc -t ../app -A app  
$ make depend  
$ make  

----

target_config.h
```c
    #define DEFAULT_ISTKSZ			(0x1800 / 4)		/* 6KByte */
```

Stack size configuration result:  
        Estimated task stack size = 128  
        Specified interrupt stack size = DEFAULT_ISTKSZ(value=1536)  
        Allocated total stack size = 1664(value=1664)  


   text |   data |  bss |   dec  |  hex | filename
--------|--------|------|--------|------|------------
  33404 |     36 | 3216 | 36656  | 8f30 | ssp.exe

