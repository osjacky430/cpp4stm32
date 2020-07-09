  #pragma once

  #define COMPILE_TIME_CLOCK_CONFIG true

  #define HSE_CLK_FREQ (8000000U)
  #define LSE_CLK_FREQ (32768U)
  #define HSI_CLK_FREQ (16000000U)
  #define LSI_CLK_FREQ (32000U)
  #define SYS_CLK_FREQ (180000000U)
  #define AHB_CLK_FREQ (180000000U)
  #define APB1_CLK_FREQ (45000000U)
  #define APB2_CLK_FREQ (90000000U)

  #define HSE_BYPASS_CLK_SRC true
  #define LSE_BYPASS_CLK_SRC false
