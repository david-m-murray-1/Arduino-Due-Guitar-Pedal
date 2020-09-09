#ifndef _SPI_CLASS_H_
#define _SPI_CLASS_H_

#include "asf.h"


class hwspi {
public:
	//friend Display;
	//friend Touch;
	//friend Codec;
	
	uint8_t chipselect:2;
	
	uint8_t spi_chipselect_display = 0; // CS pin 10 -- PA28
	uint8_t spi_chipselect_touch = 1;	// CS pin 4 -- PA29
	uint8_t spi_chipselect_codec = 2; // CS pin 52 -- PB21

 	uint8_t spimode = 0x02;

	void spi_write(uint8_t chipselect, uint8_t data_len, uint_fast16_t wData);
	
	uint_fast16_t spi_read();
	
	inline uint32_t spi_is_tx_empty(Spi *p_spi);

	void configure_pin(const uint32_t dwMask);
	
	/// \brief
	/// hardware spi constructor
	/// \param chipselect the used cs\n
	/// cs pins are:\n
	/// NPCS0 = PA28 Peripheral A = d10\n
	/// NPCS1 = PA29 Peripheral A = d4\n
	/// NPCS2 = PA30 Peripheral A = nc\n
	/// NPCS3 = PA31 Peripheral A = nc\n
	/// \param spimode the desired spi mode
	/// \param spi_divider the spi divider to calculate the spi speed\n
	/// 84 mhz / 21 = 4mhz\n
	/// \param loopback to enable or disable the loopback in hardware\n
	/// this connects the mosi to the miso if enabled\n
	hwspi(uint8_t chipselect, uint8_t spimode = 0x02):chipselect(chipselect), spimode(spimode)
	{
		// disable write protection spi
		REG_SPI0_WPMR=0x53504900;

		// disable spi
		SPI0->SPI_CR = SPI_CR_SPIDIS;
		// CS1 is pin 4 on the due
		configure_pin(PIO_PA28A_SPI0_NPCS0);		// CS: 0, pin 10 -- PA28 ---- ili9341 Display Control
		configure_pin(PIO_PA29A_SPI0_NPCS1);		// CS: 1, pin 4 -- PA29 ---- ili9341 Touch Control
		configure_pin(PIO_PB21B_SPI0_NPCS2);		// CS: 2, pin 52 -- PB21 ---- CS4271 Codec Control
	

		// configure miso, mosi and clk pins
		configure_pin(PIO_PA25A_SPI0_MISO);
		configure_pin(PIO_PA26A_SPI0_MOSI);
		configure_pin(PIO_PA27A_SPI0_SPCK);

		// enable pheripheral clock
		PMC->PMC_PCER0 = 1 << ID_SPI0;

		// Execute a software reset of the SPI twice
		SPI0->SPI_CR = SPI_CR_SWRST;
		SPI0->SPI_CR = SPI_CR_SWRST;

		// Set SPI configuration parameters.
		//int baud_div = div_ceil(sysclk_get_peripheral_hz(), baud_rate);
		SPI0->SPI_MR = SPI_MR_MSTR | SPI_MR_PS | SPI_MR_MODFDIS | 1 << 7;
		SPI0->SPI_CSR[spi_chipselect_display] = SPI_CSR_SCBR(16) | SPI_CSR_DLYBCT(0) | SPI_CSR_DLYBS(1) | SPI_CSR_CSAAT | SPI_CSR_CPOL;		// set display spi settings
		SPI0->SPI_CSR[spi_chipselect_touch] = SPI_CSR_SCBR(16) | SPI_CSR_DLYBCT(0) | SPI_CSR_DLYBS(1) | SPI_CSR_CSAAT | SPI_CSR_CPOL;		// set touch spi settings
		SPI0->SPI_CSR[spi_chipselect_codec] = SPI_CSR_SCBR(16) | SPI_CSR_DLYBCT(0) | SPI_CSR_DLYBS(1) | SPI_CSR_CSAAT | SPI_CSR_CPOL;		// set codec spi settings
	}
};

#endif