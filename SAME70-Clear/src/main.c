/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/*
  Arduino Mario Bros Tunes
  With Piezo Buzzer and PWM

  by: Dipto Pratyaksa
  last updated: 31/3/13
*/

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"
#include "consts.h"

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

typedef struct{
	int num_mus;
	char musica;
} Musica;

volatile char but_flag;

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void but_callback(void);
void buzz(Musica musica,long frequency, long length);
void sing(Musica musica);
void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

void but_callback(void){
	but_flag = !but_flag;
}

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void buzz(Musica musica, long frequency, long length){
		
	pio_clear(BUZ_PIO, BUZ_PIO_IDX_MASK);
	
	if(musica.num_mus == 1){
		long delayValue = 1000000 / frequency / 2;
	
		long numCycles = frequency * length / 1000;
	
		for (long i = 0; i < numCycles; i++) {
			if(but_flag == 1){
				pio_clear(LED_PIO, LED_PIO_IDX_MASK);
				pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
				pio_set(LED_PIO, LED_PIO_IDX_MASK);
			} 
			pio_clear(LED1_PLACA_PIO, LED1_PLACA_IDX_MASK);
			pio_set(BUZ_PIO, BUZ_PIO_IDX_MASK);     
			delay_us(delayValue);                    
		
			pio_set(LED1_PLACA_PIO, LED1_PLACA_IDX_MASK);
			pio_clear(BUZ_PIO, BUZ_PIO_IDX_MASK);    
			delay_us(delayValue);                          
		}
	}
	else if(musica.num_mus == 2){
		long delayValue = 1000000 / frequency / 2;
		
		long numCycles = frequency * length / 600;
		
		for (long i = 0; i < numCycles; i++) {
			if(but_flag == 1){
				pio_clear(LED_PIO, LED_PIO_IDX_MASK);
				pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
				pio_set(LED_PIO, LED_PIO_IDX_MASK);
			}
			pio_clear(LED2_PLACA_PIO, LED2_PLACA_IDX_MASK);
			pio_set(BUZ_PIO, BUZ_PIO_IDX_MASK);     
			delay_us(delayValue);                   
			
			pio_set(LED2_PLACA_PIO, LED2_PLACA_IDX_MASK);
			pio_clear(BUZ_PIO, BUZ_PIO_IDX_MASK);   
			delay_us(delayValue);                           
		}
	}
	else if(musica.num_mus == 3){
		long delayValue = 1000000 / frequency / 2;
		
		long numCycles = frequency * length / 600;
		
		for (long i = 0; i < numCycles; i++) {
			if(but_flag == 1){
				pio_clear(LED_PIO, LED_PIO_IDX_MASK);
				pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
				pio_set(LED_PIO, LED_PIO_IDX_MASK);
			} 
			pio_clear(LED3_PLACA_PIO, LED3_PLACA_IDX_MASK);
			pio_set(BUZ_PIO, BUZ_PIO_IDX_MASK);      
			delay_us(delayValue);                   
			
			pio_set(LED3_PLACA_PIO, LED3_PLACA_IDX_MASK);
			pio_clear(BUZ_PIO, BUZ_PIO_IDX_MASK);    
			delay_us(delayValue);                        
		}
	}
	
	pio_clear(BUZ_PIO, BUZ_PIO_IDX_MASK);	
}

void sing(Musica musica){
	if(musica.num_mus == 1){
		int size = sizeof(melody) / sizeof(int);
	
		for (int thisNote = 0; thisNote < size ; thisNote ++) {
			int noteDuration = 2000 / tempo[thisNote];
		
			buzz(musica, melody[thisNote],  noteDuration);
	
			int pauseBetweenNotes = noteDuration*1.3;
			delay_us(pauseBetweenNotes);
	
			buzz(musica ,0, noteDuration);
		}		
	}
	else if(musica.num_mus == 2){
		int size = sizeof(underworld_melody) / sizeof(int);
		
		for (int thisNote = 0; thisNote < size ; thisNote ++) {
			int noteDuration = 2000 / underworld_tempo[thisNote];
			
			buzz(musica, underworld_melody[thisNote]*4,  noteDuration);
			
			int pauseBetweenNotes = noteDuration*1.3;
			delay_us(pauseBetweenNotes);
			
			buzz(musica ,0, noteDuration);
		}
	}
	else if(musica.num_mus == 3){
		int size = sizeof(pirate_notes) / sizeof(int);
		
		for (int thisNote = 0; thisNote < size ; thisNote ++) {
			int noteDuration = pirate_tempo[thisNote]*0.75;
			
			buzz(musica, pirate_notes[thisNote],  noteDuration);
			
			int pauseBetweenNotes = noteDuration*1.3;
			delay_us(pauseBetweenNotes);
			
			buzz(musica ,0, noteDuration);
		}
	}
}

// Função de inicialização do uC
void init(void){
	sysclk_init();
  
	WDT->WDT_MR = WDT_MR_WDDIS;

	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(LED3_PLACA_PIO_ID);
	pmc_enable_periph_clk(LED2_PLACA_PIO_ID);
	pmc_enable_periph_clk(LED1_PLACA_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUZ_PIO_ID);
	
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED3_PLACA_PIO, LED3_PLACA_IDX_MASK, 0, 0, 0);
	pio_set_output(LED2_PLACA_PIO, LED2_PLACA_IDX_MASK, 0, 0, 0);
	pio_set_output(LED1_PLACA_PIO, LED1_PLACA_IDX_MASK, 0, 0, 0);
	pio_set_output(LED3_PLACA_PIO, LED3_PLACA_IDX_MASK, 0, 0, 0);
	pio_set_output(BUZ_PIO, BUZ_PIO_IDX_MASK, 0, 0, 0);
	
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	pio_pull_up(BUT3_PIO, BUT3_PIO_IDX_MASK, 1);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);
	
	pio_handler_set(BUT_PIO, BUT_PIO_ID, BUT_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but_callback);
	pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
	
	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_SetPriority(BUT_PIO_ID, 4);
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

int main(void){
	init();
	
	but_flag = false;
	
	Musica musica;
	
	while(1){		
		pio_set(LED3_PLACA_PIO, LED3_PLACA_IDX_MASK);
		pio_set(LED2_PLACA_PIO, LED2_PLACA_IDX_MASK);
		pio_set(LED1_PLACA_PIO, LED1_PLACA_IDX_MASK);
		pio_set(LED_PIO, LED_PIO_IDX_MASK);
		
		if(pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)==0){
			musica.num_mus = 1;
			musica.musica = "Mario Main";
			
			sing(musica);
		}
		
		else if(pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)==0){
			musica.num_mus = 2;
			musica.musica = "Underworld Mario";
			
			sing(musica);
		}
		
		else if(pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK)==0){
			musica.num_mus = 3;
			musica.musica = "Pirates";
			
			sing(musica);
		}
	}
	
	return 0;
}
