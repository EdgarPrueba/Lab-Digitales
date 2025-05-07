// Definición de direcciones de memoria de periféricos
#define GPIO_SWs    0x80001400      // Dirección de entrada de los switches
#define GPIO_LEDs   0x80001404      // Dirección de salida para encender LEDs
#define GPIO_INOUT  0x80001408      // Dirección para configurar los GPIO como entrada/salida

// Temporizador PWM para color azul
#define RPTC2_CNTR   0x80001240
#define RPTC2_HRC    0x80001244     // Registro de comparación alto
#define RPTC2_LRC    0x80001248     // Registro de comparación bajo
#define RPTC2_CTRL   0x8000124c     // Control del temporizador

// Temporizador PWM para color verde
#define RPTC3_CNTR   0x80001280
#define RPTC3_HRC    0x80001284
#define RPTC3_LRC    0x80001288
#define RPTC3_CTRL   0x8000128c

// Temporizador PWM para color rojo
#define RPTC4_CNTR   0x800012c0
#define RPTC4_HRC    0x800012c4
#define RPTC4_LRC    0x800012c8
#define RPTC4_CTRL   0x800012cc

// Macros para lectura y escritura de registros de memoria-mapeada
#define READ_Reg(dir) (*(volatile unsigned *)dir)
#define WRITE_Reg(dir, value) { (*(volatile unsigned *)dir) = (value); }

int main(void) {
    int switches_value, switches_value_temp, counter_value, delay = 100000, i;

    // Configura todos los GPIOs como entradas (0xFFFF)
        WRITE_Reg(GPIO_INOUT, 0xFFFF);

    while (1) {
        // Lee el valor de los 16 switches (bits [31:16] de la dirección)
        switches_value = READ_Reg(GPIO_SWs);
        switches_value = switches_value >> 16;

        // Refleja el valor de los switches en los LEDs para visualización
        WRITE_Reg(GPIO_LEDs, switches_value);

        // ---------------- PWM AZUL (bits [4:0]) ----------------
        WRITE_Reg(RPTC2_HRC, 0xF);  // Ciclo alto fijo
        switches_value_temp = switches_value & 0x1F;  // Extrae bits 0-4
        switches_value_temp = switches_value_temp + 0xF; // Ajusta ciclo bajo
        WRITE_Reg(RPTC2_LRC, switches_value_temp);    // Configura LRC
        WRITE_Reg(RPTC2_CTRL, 0x80);  // Reset del temporizador
        WRITE_Reg(RPTC2_CTRL, 0x9);   // Enable y modo PWM

        // ---------------- PWM VERDE (bits [9:5]) ----------------
        WRITE_Reg(RPTC3_HRC, 0xF);  // Ciclo alto fijo
        switches_value_temp = (switches_value >> 5) & 0x1F;  // Extrae bits 5-9
        switches_value_temp = switches_value_temp + 0xF;
        WRITE_Reg(RPTC3_LRC, switches_value_temp);
        WRITE_Reg(RPTC3_CTRL, 0x80);  // Reset
        WRITE_Reg(RPTC3_CTRL, 0x9);   // Enable

        // ---------------- PWM ROJO (bits [14:10]) ----------------
        WRITE_Reg(RPTC4_HRC, 0xF);  // Ciclo alto fijo
        switches_value_temp = (switches_value >> 10) & 0x1F;  // Extrae bits 10-14
        switches_value_temp = switches_value_temp + 0xF;
        WRITE_Reg(RPTC4_LRC, switches_value_temp);
        WRITE_Reg(RPTC4_CTRL, 0x80);  // Reset
        WRITE_Reg(RPTC4_CTRL, 0x9);   // Enable

        // Espera para permitir que los cambios de ciclo sean visibles
        for (i = 0; i < delay; i++);
    }

    return 0;
}