#include "FreeRTOS.h"

#ifndef configMEMMANG_HEAP
#define configMEMMANG_HEAP      4
#endif

#if (configMEMMANG_HEAP == 1)
#include "../FreeRTOS/Source/portable/MemMang/heap_1.c"
#elif (configMEMMANG_HEAP == 2)
#include "../FreeRTOS/Source/portable/MemMang/heap_2.c"
#elif (configMEMMANG_HEAP == 3)
#include "../FreeRTOS/Source/portable/MemMang/heap_3.c"
#elif (configMEMMANG_HEAP == 4)
#include "../FreeRTOS/Source/portable/MemMang/heap_4.c"
#elif (configMEMMANG_HEAP == 5)
#include "../FreeRTOS/Source/portable/MemMang/heap_5.c"
#else
#error "Wrong Memory allocation implementations defined"
#endif
