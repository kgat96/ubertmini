/*
 * This file is part of the ubertoothp project.
 *
 * Copyright (C) 2016 Kage Shen <kgat96@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

#include <libopencm3/stm32/dma.h>

#include "config.h"
#include "cc.h"
//#include "usbhid.h"

void kputhex(unsigned int value, int digits)
{
    while (digits-- > 0) {
        unsigned int tmp = (value >> (4 * digits)) & 0xf;
        kputc(tmp > 9 ? tmp - 10 + 'a' : tmp + '0');
    }
}

void kputs(char *s)
{
    while (*s) {
        if (*s == '\n')
            kputc('\r');
        kputc(*s++);
    }
}

void delay(void)
{
    u32 i;
    for (i = 0; i < 1000000; i++) { /* Wait a bit. */
        __asm__("nop");
    }
}


static void usart_setup(void)
{
    /* Enable clocks for USART1. */
    rcc_periph_clock_enable(RCC_USART1);

    /* Setup USART1 parameters. */
    usart_set_baudrate(nUSART, 115200);
    usart_set_databits(nUSART, 8);
    usart_set_stopbits(nUSART, USART_STOPBITS_1);
    usart_set_mode(nUSART, USART_MODE_TX);
    usart_set_parity(nUSART, USART_PARITY_NONE);
    usart_set_flow_control(nUSART, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(nUSART);
}

static void gpio_setup(void)
{
    /* Enable GPIO clock. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);

    /* Set GPIO LED. */
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO8); // LED1
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO7); // LED2
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO6); // LED3
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO15); // LED4

    /* Setup GPIO pins for USART1 transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO9);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO10);

    /* Setup USART1 TX/RX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);   // tx
    gpio_set_af(GPIOA, GPIO_AF7, GPIO10);  // rx

    /* set certain pins as outputs, all others inputs */
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, PIN_MOSI);
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP,
            PIN_TX | PIN_CC3V3 | PIN_RX | PIN_BTGR);
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP,
            PIN_USRLED | PIN_RXLED | PIN_TXLED | PIN_CSN | PIN_SCLK | PIN_PAEN | PIN_HGM);

    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, PIN_GIO6);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, PIN_GIO1);

    BTGR_CLR(); TX_CLR(); RX_CLR();

    /* USB pins */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11 | GPIO12 );
    gpio_set_af(GPIOA, GPIO_AF10, GPIO11 | GPIO12 );

}

static void spi_setup(void)
{
    /*
      * Chart of the various SPI ports (1 - 6) and where their pins can be:
      *
      *       NSS          SCK                   MISO                    MOSI
      *   --------------   -------------------   -------------           ---------------
      * SPI1  PA4, PA15    PA5, PB3              PA6, PB4                PA7, PB5
      * SPI2  PB9, PB12,   PI0                   PB10, PB13, PD3, PI1    PB14, PC2, PI2  PB15, PC3, PI3
      * SPI3  PA15*, PA4*  PB3*, PC10*           PB4*, PC11*             PB5*, PD6, PC12*
      * SPI4  PE4,PE11     PE2, PE12             PE5, PE13               PE6, PE14
      * SPI5  PF6, PH5     PF7, PH6              PF8                     PF9, PF11, PH7
      * SPI6  PG8          PG13                  PG12                    PG14
      *
      * Pin name with * is alternate function 6 otherwise use alternate function 5.
      *
      * MEMS uses SPI5 - SCK (PF7), MISO (PF8), MOSI (PF9),
      * MEMS CS* (PC1)  -- GPIO
      * MEMS INT1 = PA1, MEMS INT2 = PA2
      */

     // SPI3 configure
     gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
     gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO1);

     // set debug gpio 4 PA15 to NSS(cs)
     gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO15);
     gpio_set_af(GPIOA, GPIO_AF6, GPIO15);
     // set spi sck PB3
     gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO3);
     gpio_set_af(GPIOB, GPIO_AF6, GPIO3);
     // set spi MISO PB4
     gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO4);
     gpio_set_af(GPIOB, GPIO_AF6, GPIO4);
     // set spi MoSI PB5
     //gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO5);
     //gpio_set_af(GPIOB, GPIO_AF6, GPIO5);

     rcc_periph_clock_enable(RCC_SPI3);

     // slave mode, 8 bit, one line only,
     SPI3_CR1 = SPI_CR1_BIDIMODE_1LINE_BIDIR |
                     SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL; // SPI_CR1_BIDIOE
     //SPI3_CR1 =  SPI_CR1_SSM | SPI_CR1_CPHA | SPI_CR1_CPOL;
     // RX dma mode
     SPI3_CR2 |= 1;
}

static void tim_setup(void)
{
    /* Enable TIM2 clock. */
    rcc_periph_clock_enable(RCC_TIM2);

    /* Enable TIM2 interrupt. */
    nvic_enable_irq(NVIC_TIM2_IRQ);

    /* Reset TIM2 peripheral. */
    timer_reset(TIM2);

    /* Timer global mode:
     * - No divider
     * - Alignment edge
     * - Direction up
     */
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
               TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    /* Reset prescaler value.
     * Running the clock at 5kHz.
     */
    /*
     * On STM32F4 the timers are not running directly from pure APB1 or
     * APB2 clock busses.  The APB1 and APB2 clocks used for timers might
     * be the double of the APB1 and APB2 clocks.  This depends on the
     * setting in DCKCFGR register. By default the behaviour is the
     * following: If the Prescaler APBx is greater than 1 the derived timer
     * APBx clocks will be double of the original APBx frequencies. Only if
     * the APBx prescaler is set to 1 the derived timer APBx will equal the
     * original APBx frequencies.
     *
     * In our case here the APB1 is devided by 4 system frequency and APB2
     * divided by 2. This means APB1 timer will be 2 x APB1 and APB2 will
     * be 2 x APB2. So when we try to calculate the prescaler value we have
     * to use rcc_apb1_freqency * 2!!!
     *
     * For additional information see reference manual for the stm32f4
     * familiy of chips. Page 204 and 213
     */
    //timer_set_prescaler(TIM2, ((rcc_apb1_frequency * 2) / 100000) - 0);
    timer_set_prescaler(TIM2, 5); // rcc_apb1_frequency * 2 / (5 + 1) = 10MHz

    /* Enable preload. */
    timer_disable_preload(TIM2);

    /* Continous mode. */
    timer_continuous_mode(TIM2);

    /* Period. */
    //timer_set_period(TIM2, 3276800000 - 1);
    timer_set_period(TIM2, 1000000 - 1);

    /* Disable outputs. */
    timer_disable_oc_output(TIM2, TIM_OC1);
    timer_disable_oc_output(TIM2, TIM_OC2);
    timer_disable_oc_output(TIM2, TIM_OC3);
    timer_disable_oc_output(TIM2, TIM_OC4);

    /* -- OC1 configuration -- */
#if 0
    /* Configure global mode of line 1. */
    timer_disable_oc_clear(TIM2, TIM_OC1);
    timer_disable_oc_preload(TIM2, TIM_OC1);
    timer_set_oc_slow_mode(TIM2, TIM_OC1);
    timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_FROZEN);

    /* Set the capture compare value for OC1. */
    timer_set_oc_value(TIM2, TIM_OC1, 1000);

    /* Enable commutation interrupt. */
    timer_enable_irq(TIM2, TIM_DIER_CC1IE);
#endif
    /* ---- */

    /* ARR reload enable. */
    timer_disable_preload(TIM2);

    /* Counter enable. */
    timer_enable_counter(TIM2);

    /* Enable commutation interrupt. */
    timer_enable_irq(TIM2, TIM_DIER_UIE);
}

#define UES_DMA_STREAM DMA_STREAM0
#define UES_DMA_CONUR  DMA1

static void dma_setup(void)
{
    /* DAC channel 1 uses DMA controller 1 Stream 5 Channel 7. */
    /* Enable DMA1 clock and IRQ */
    rcc_periph_clock_enable(RCC_DMA1);
    nvic_enable_irq(NVIC_DMA1_STREAM0_IRQ);

    dma_stream_reset(UES_DMA_CONUR, UES_DMA_STREAM);
    dma_set_priority(UES_DMA_CONUR, UES_DMA_STREAM, DMA_SxCR_PL_LOW);
    dma_set_memory_size(UES_DMA_CONUR, UES_DMA_STREAM, DMA_SxCR_MSIZE_8BIT);
    dma_set_peripheral_size(UES_DMA_CONUR, UES_DMA_STREAM, DMA_SxCR_PSIZE_8BIT);
    dma_enable_memory_increment_mode(UES_DMA_CONUR, UES_DMA_STREAM);
    dma_enable_circular_mode(UES_DMA_CONUR, UES_DMA_STREAM);
    dma_set_transfer_mode(UES_DMA_CONUR, UES_DMA_STREAM, DMA_SxCR_DIR_PERIPHERAL_TO_MEM);

    /* The register to target is the DAC1 8-bit right justified data register */
    dma_set_peripheral_address(UES_DMA_CONUR, UES_DMA_STREAM, (uint32_t) &SPI3_DR);
    /* The array v[] is filled with the waveform data to be output */
    dma_set_memory_address(UES_DMA_CONUR, UES_DMA_STREAM, (uint32_t) rxbuf1);
    dma_set_memory_address_1(UES_DMA_CONUR, UES_DMA_STREAM, (uint32_t) rxbuf2);
    dma_set_number_of_data(UES_DMA_CONUR, UES_DMA_STREAM, DMA_SIZE);

    dma_channel_select(UES_DMA_CONUR, UES_DMA_STREAM, DMA_SxCR_CHSEL_0);
    dma_enable_double_buffer_mode(UES_DMA_CONUR, UES_DMA_STREAM);

    dma_enable_transfer_complete_interrupt(UES_DMA_CONUR, UES_DMA_STREAM);
    dma_enable_transfer_error_interrupt(UES_DMA_CONUR, UES_DMA_STREAM);

    dma_enable_stream(UES_DMA_CONUR, UES_DMA_STREAM);
}

/* an ugly but effective way to identify a GIAC (inquiry packet) */
static int find_giac(u8 *buf)
{
    int i, j;
    const uint8_t giac[8][7] = {
            {0x47, 0x5c, 0x58, 0xcc, 0x73, 0x34, 0x5e},
            {0x8e, 0xb8, 0xb1, 0x98, 0xe6, 0x68, 0xbc},
            {0x11, 0xd7, 0x16, 0x33, 0x1c, 0xcd, 0x17},
            {0x23, 0xae, 0x2c, 0x66, 0x39, 0x9a, 0x2f},
            {0x75, 0xc5, 0x8c, 0xc7, 0x33, 0x45, 0xe7},
            {0xeb, 0x8b, 0x19, 0x8e, 0x66, 0x8b, 0xce},
            {0x1d, 0x71, 0x63, 0x31, 0xcc, 0xd1, 0x79},
            {0x3a, 0xe2, 0xc6, 0x63, 0x99, 0xa2, 0xf3}};

    for (i = 0; i < (DMA_SIZE - 6); i++)
            for (j = 0; j < 8; j++)
                if (buf[i] == giac[j][0]
                        && buf[i + 1] == giac[j][1]
                        && buf[i + 2] == giac[j][2]
                        && buf[i + 3] == giac[j][3]
                        && buf[i + 4] == giac[j][4]
                        && buf[i + 5] == giac[j][5]
                        && buf[i + 6] == giac[j][6])
                    return 1;

    return 0;
}

int main(void)
{
    gpio_setup();

    usart_setup();

    kputs("\nSpectrum start\n");

    cc_reset();

    cc_init();

    delay(); // why ???

    /* Enable external high-speed oscillator 16MHz. */
    //rcc_osc_bypass_enable(RCC_HSE);
    //rcc_clock_setup_hse_3v3(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_120MHZ]);

    //rcc_periph_clock_enable(RCC_OTGFS);

    /* Setup USART1 parameters. */
    //usart_set_baudrate(USART1, 2000000);

    kputs("system clock init done!\n");

    tim_setup();

    spi_setup();

    dma_setup();

    //usb_setup();

    SPI_CR1(SPI3) |= SPI_CR1_SPE;

    cc_rx_mode();
    //cc_specan_mode();

    while (1) {
//        kputhex(idle_rxbuf[0], 2);
//        kputhex(idle_rxbuf[1], 2);
//        kputhex(idle_rxbuf[2], 2);
//        kputhex(idle_rxbuf[3], 2);

        if (find_giac(idle_rxbuf)) {
            gpio_toggle(GPIOC, PIN_RXLED); /* LED on/off */
            kputc('*');
        }
    }

    while(1) {

        u8 spbuff[64];

        get_specan_date(spbuff, 64);
        //usb_write_packet(spbuff, 64);

    }

    while (1) {
        /* Manually: */
        /* Using API functions gpio_set()/gpio_clear(): */
        /* Using API function gpio_toggle(): */

        gpio_set(GPIOC, GPIO0); /* LED on/off */
        gpio_set(GPIOB, GPIO15); /* LED on/off */
        delay();
        gpio_clear(GPIOC, GPIO0); /* LED on/off */
        gpio_clear(GPIOB, GPIO15); /* LED on/off */
        delay();

    }

    return 0;
}

#define CLK100NS TIM_CNT(TIM2)
volatile u8 clkn_high;
#define CLKN ((clkn_high << 20) | (CLK100NS / 3125))

/* clkn_high is incremented each time CLK100NS rolls over */
void tim2_isr(void)
{
    //kputs("tim2_isr\n");

    if (TIM_SR(TIM2) & TIM_SR_UIF) {
        /* Clear compare interrupt flag. */
        TIM_SR(TIM2) = ~TIM_SR_UIF;
        ++clkn_high;

        if (gpio_get(GPIOB, PIN_GIO1))
            kputc('1');
        else
            kputc('0');

        gpio_toggle(GPIOC, PIN_USRLED);
    }

}

/*--------------------------------------------------------------------*/
/* The ISR simply provides a test output for a CRO trigger */

void dma1_stream0_isr(void)
{
    if (dma_get_interrupt_flag(UES_DMA_CONUR, UES_DMA_STREAM, DMA_TCIF)) {
        dma_clear_interrupt_flags(UES_DMA_CONUR, UES_DMA_STREAM, DMA_TCIF);
        gpio_toggle(GPIOC, GPIO7);

        if (DMA_SCR(UES_DMA_CONUR, UES_DMA_STREAM) & DMA_SxCR_CT) {
            active_rxbuf = &rxbuf2[0];
            idle_rxbuf = &rxbuf1[0];
        } else {
            active_rxbuf = &rxbuf1[0];
            idle_rxbuf = &rxbuf2[0];
        }

    } else if (dma_get_interrupt_flag(UES_DMA_CONUR, UES_DMA_STREAM, DMA_TEIF)) {
        dma_clear_interrupt_flags(UES_DMA_CONUR, UES_DMA_STREAM, DMA_TEIF);
        kputc('t');
    }
}


