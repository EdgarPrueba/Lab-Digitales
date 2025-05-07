#define GPIO_SWs    0x80001400
#define GPIO_LEDs   0x80001404
#define GPIO_INOUT  0x80001408

#define GPIO_BTNs  0x80001800
#define BTN_INOUT 0x80001808

#define SlowDelay   10000000
#define FastDelay   SlowDelay/10


.globl main

main:

    li t0, 0
    li t5, SlowDelay // Cargar el delay lento

    li  a0,  GPIO_INOUT // Definir los Leds como salidas
    li  t1, 0x0000FFFF  
    sw  t1, 0(a0)

    li  a0,  BTN_INOUT // Definir los botones como entradas 
    li  t1, 0x0
    sw  t1, 0(a0)

repetir:


    li  a1, GPIO_BTNs // Leer las entradas de los botones
    lw  t1, 0(a1)
    and t2, t1, 1 // obtener el valor del centro
    beq t2, zero, slow // Si no es presionado, el delay es lento

    li  t5, FastDelay // si se presiona se acelera la cuenta
    j fast
slow:
    li t5, SlowDelay
fast:

    li  a1, GPIO_BTNs // Leer las entradas de los botones       
    lw  t1, 0(a1)

    and t2, t1, 2  // Obtener el valor del boton de arriba         
    beq t2, zero, continuar_cuenta // Si no se presiona se sigue la cuenta
    li  t0, 0  // Si se presiona se reinicia la cuenta
continuar_cuenta:
 
    li  a0, GPIO_LEDs  // Poner el valor de la cuenta en los leds    
    sw  t0, 0(a0)
 
    add  t3, zero, t5 // Pasar el valor del delay a t3
    
delay:
        add t3, t3, -1  // Aplicar delay
        bge t3, zero, delay

    add t0, t0, 1 // aumentar cuenta

    j repetir 

.end