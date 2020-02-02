#ifdef __cplusplus
extern "C" {
#endif

/*  -----------------------------------------------------------
    Port specific definitions.

    The settings in this file configure FreeRTOS correctly for the
    given hardware and compiler.

    These settings should not be altered.
    -----------------------------------------------------------
*/

/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	uint32_t
#define portBASE_TYPE	long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
typedef uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffff
#else
typedef uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000    / configTICK_RATE_HZ )
#define portTICK_PERIOD_US			( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			8
/*-----------------------------------------------------------*/


/* Scheduler utilities. */
extern void vPortYield(void);
#define portNVIC_INT_CTRL_REG		( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT		( 1UL << 28UL )
#define portYIELD()					vPortYield()
#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired ) portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/


/* Critical section management. */
extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);
extern uint32_t ulSetInterruptMaskFromISR(void) __attribute__((naked));
extern void vClearInterruptMaskFromISR(uint32_t ulMask)  __attribute__((naked));

#define portSET_INTERRUPT_MASK_FROM_ISR()		ulSetInterruptMaskFromISR()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vClearInterruptMaskFromISR( x )
#define portDISABLE_INTERRUPTS()				__asm volatile 	( " cpsid i " )
#define portENABLE_INTERRUPTS()					__asm volatile 	( " cpsie i " )
#define portENTER_CRITICAL()					vPortEnterCritical()
#define portEXIT_CRITICAL()						vPortExitCritical()

/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#define portNOP()

#ifdef __cplusplus
}
#endif

