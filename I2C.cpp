#define SLAVE_ADDRESS 0x3C

int main(){

  sysclk_init();
  board_init();
  /* Enable the peripheral clock for TWI */
  pmc_enable_periph_clk(BOARD_ID_TWI_MASTER);
  
	twi_master_init(BOARD_BASE_TWI_MASTER, SLAVE_ADDRESS);

	/* Clear receipt buffer */
	twi_read_byte();
	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
	twi_enable_interrupt(BOARD_BASE_TWI_MASTER, /*TWI_SR_SVACC*/);
  
