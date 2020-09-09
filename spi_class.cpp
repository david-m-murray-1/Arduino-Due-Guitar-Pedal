/*
 * spi_test.cpp
 *
 * Created: 6/13/2020 8:45:53 PM
 *  Author: davem
 */ 
#include "spi_class.h"

void hwspi::spi_write(uint8_t chipselect, uint8_t data_len, uint_fast16_t wData){
	SPI0->SPI_CSR[chipselect] |= (SPI_CSR_BITS_Msk & (data_len << SPI_CSR_BITS_Pos));
	SPI0->SPI_CR = SPI_CR_SPIEN;
	while ( (SPI0->SPI_SR & SPI_SR_TXEMPTY) == 0 );
	SPI0->SPI_TDR = wData | SPI_MR_PCS( chipselect );
	while ( (SPI0->SPI_SR & SPI_SR_TDRE) == 0 );
	SPI0->SPI_CR = SPI_CR_SPIDIS;
}

uint_fast16_t hwspi::spi_read(){
	SPI0->SPI_CR = SPI_CR_SPIEN;
	while ( (SPI0->SPI_SR & SPI_SR_RDRF) == 0 ) ;
	return SPI0->SPI_RDR & 0xFFFF;
	SPI0->SPI_CR = SPI_CR_SPIDIS;
}

inline uint32_t hwspi::spi_is_tx_empty(Spi *p_spi)
{
	if (p_spi->SPI_SR & SPI_SR_TXEMPTY) {
		return 1;
		} else {
		return 0;
	}
}

void hwspi::configure_pin(const uint32_t dwMask) {
	uint32_t dwSR;

	PIOA->PIO_IDR = dwMask;
	dwSR = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR &= (~dwMask & dwSR);
	PIOA->PIO_PDR = dwMask;

	/* Disable interrupts on the pin(s) */
	PIOA->PIO_IDR = dwMask;
	// disable pull ups
	PIOA->PIO_PUDR = dwMask;
}