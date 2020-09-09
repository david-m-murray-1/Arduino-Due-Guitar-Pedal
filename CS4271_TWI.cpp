#include "CS4271_TWI.h"

// Setup I2C address for codec
#define CODEC_ADDR 0x10


void AudioControlCS4271::codec_write(uint8_t reg, uint8_t data)
{
	// For CS4272 all data is written between single
	// start/stop sequence
	
	uint8_t buf[2];
	
	buf[0] = reg;
	buf[1] = data;

	send_package(buf[1],CODEC_ADDR);

}

uint8_t AudioControlCS4271::codec_read(uint8_t reg)
{
	// No waveform demo for read,
	// so assume first write MAP,
	// then rep-start (or stop and start),
	// the read
	
	send_package(0xFF,CODEC_ADDR);

	uint8_t buf;
	if(send_package(CODEC_ADDR,1,&buf) != 1)
	{
		return 0;
	}

	return buf;
}

void AudioControlCS4271::init() {	
	// Initialize I2C
	twi_init();
	delay_ms(100);

	// Setup Reset pin (GPIO)
	// Right now assuming that we're using Teensy pin 2
	// which is Port D pin 0
	
	// Setup Pin muxing for GPIO (alt 1)
	PORTD_PCR0 = PORT_PCR_MUX(1);

	// Setup pin for digital out
	GPIOD_PDDR |= (1 << 0);

	// Make sure pin is cleared (still driving reset)
	GPIOD_PCOR = (1 << 0);

	delay_ms(1);
	
	// Release Reset (drive pin high)
	GPIOD_PSOR = (1 << 0);
	
	// Wait for ~2-5ms (1-10 ms time window spec'd in datasheet)
	delay_ms(2);
	
	// Set power down and control port enable as spec'd in the
	// datasheet for control port mode
	codec_write(CS4271_MODE_CTRL2, CODEC_MODE_CTRL2_POWER_DOWN
	| CODEC_MODE_CTRL2_CTRL_PORT_EN);
	
	// Further setup
	delay_ms(1);

	// Set ratio select for MCLK=512*LRCLK (BCLK = 64*LRCLK), and master mode
	codec_write(CODEC_MODE_CONTROL, CODEC_MC_RATIO_SEL(2) | CODEC_MC_MASTER_SLAVE);

	delay_ms(10);
	
	// Release power down bit to start up codec
	codec_write(CODEC_MODE_CTRL2, CODEC_MODE_CTRL2_CTRL_PORT_EN);
	
	// Wait for everything to come up
	delay_ms(10);
}

//Write a byte to a specified register on the MPU9250
uint8_t AudioControlCS4271::WriteCS4271Register(uint8_t mpuRegister, uint8_t value)
{
	//Configure packet
	twiBuffer[0] = value;                   //Value to write
	twiPacket.length = 1;                   //1 byte to write
	twiPacket.addr[0] = mpuRegister;        //Write Register Address
	twiPacket.addr_length = 1;              //Single byte register address
	//Perform write operation
	return(twi_master_write(TWI1,&twiPacket));
}

//Read a byte from a specified register on the MPU9250
uint8_t AudioControlCS4271::ReadCS4271Register(uint8_t CS4271Register)
{
	//Configure packet
	twiPacket.length = 1;                   //1 byte to read
	twiPacket.addr[0] = CS4271Register;        //Read Register Address
	twiPacket.addr_length = 1;              //Single byte register address
	//Perform read operation
	return(twi_master_read(TWI1,&twiPacket));
}

//Initialize MPU9250 IMU
bool AudioControlCS4271::InitCS4271(void)
{
	//Setup global TWI Comms Packet
	twiPacket.buffer = &twiBuffer;                  //Read/write buffer
	twiPacket.length = 1;                           //#bytes to tx/rx
	twiPacket.chip = (uint32_t) CS4271_I2C_ADDR;   //Slave address
	twiPacket.addr[0] = 0;                          //Read/Write Register Address
	twiPacket.addr_length = 0;                      //Internal Address length

	return true;
}

void AudioControlCS4271::SetupPeripherals(void)
{
        //Disable watchdog timer
        wdt_disable(WDT);
        pio_set_peripheral(PIOB, PIO_PERIPH_A, PIO_PB12A_TWD1 | PIO_PB13A_TWCK1);

        //Enable TWI1 Clock
        sysclk_enable_peripheral_clock(ID_TWI1);

        //Configure TWI options - 10MHz bus speed
        const twi_options_t twiOpts = {sysclk_get_cpu_hz(), 10000000UL, 0, 0};

        //Initialize TWI module as a master
        twi_master_init(TWI1, &twiOpts);
}

bool AudioControlCS4271::enable(void)
{
	
	return true;
}

bool AudioControlCS4271::volumeInteger(unsigned int n)
{
	unsigned int val = 0x7F - (n & 0x7F);
	// channel A
	codec_write(CS4271_DAC_CHA_VOL,CS4271_DAC_CHA_VOL_VOLUME(val));
	// channel B
	write(CS4271_DAC_CHB_VOL,CS4271_DAC_CHB_VOL_VOLUME(val));
	return true;
}

bool AudioControlCS4271::volume(float left, float right)
{
	unsigned int leftInt,rightInt;

	leftInt = left*127 + 0.499;
	rightInt = right*127 + 0.499;

	unsigned int val = 0x7F - (leftInt & 0x7F);
	write(CS4271_DAC_CHA_VOL,CS4271_DAC_CHA_VOL_VOLUME(val));
	
	val = 0x7F - (rightInt & 0x7F);
	write(CS4271_DAC_CHB_VOL,CS4271_DAC_CHB_VOL_VOLUME(val));

	return true;
}

bool AudioControlCS4271::dacVolume(float left, float right)
{
	return volume(left,right);
}

bool AudioControlCS4271::muteOutput(void)
{
	write(CS4271_DAC_CHA_VOL,
	regLocal[CS4271_DAC_CHA_VOL] | CS4271_DAC_CHA_VOL_MUTE);

	write(CS4271_DAC_CHB_VOL,
	regLocal[CS4271_DAC_CHB_VOL] | CS4271_DAC_CHB_VOL_MUTE);

	return true;
}

bool AudioControlCS4271::unmuteOutput(void)
{
	write(CS4271_DAC_CHA_VOL,
	regLocal[CS4271_DAC_CHA_VOL] & ~CS4271_DAC_CHA_VOL_MUTE);

	write(CS4271_DAC_CHB_VOL,
	regLocal[CS4271_DAC_CHB_VOL] & ~CS4271_DAC_CHB_VOL_MUTE);

	return true;
}

bool AudioControlCS4271::muteInput(void)
{
	uint8_t val = regLocal[CS4271_ADC_CTRL] | CS4271_ADC_CTRL_MUTE(3);
	write(CS4271_ADC_CTRL,val);
	return true;
}

bool AudioControlCS4271::unmuteInput(void)
{
	uint8_t val = regLocal[CS4271_ADC_CTRL] & ~CS4271_ADC_CTRL_MUTE(3);
	write(CS4271_ADC_CTRL,val);
	return true;
}

bool AudioControlCS4271::enableDither(void)
{
	uint8_t val = regLocal[CS4271_ADC_CTRL] | CS4271_ADC_CTRL_DITHER;
	write(CS4272_ADC_CTRL,val);
	return true;
}

bool AudioControlCS4271::disableDither(void)
{
	uint8_t val = regLocal[CS4271_ADC_CTRL] & ~CS4271_ADC_CTRL_DITHER;
	send_package(val, CS4271_ADC_CTRL);
	return true;
}

bool AudioControlCS4271::write(unsigned int reg, unsigned int val)
{
	// Write local copy first
	if(reg > 7)
	return false;

	regLocal[reg] = val;

	send_package(CS4271_I2C_ADDR, )
	Wire.beginTransmission(CS4272_ADDR);
	Wire.write(reg & 0xFF);
	Wire.write(val & 0xFF);
	Wire.endTransmission();
	return true;
}


// Initialize local registers to CS4272 reset status
void AudioControlCS4271::initLocalRegs(void)
{
	regLocal[CS4271_MODE_CONTROL] = 0x00;
	regLocal[CS4271_DAC_CONTROL] = CS4271_DAC_CTRL_AUTO_MUTE;
	regLocal[CS4271_DAC_VOL] = CS4271_DAC_VOL_SOFT_RAMP(2) | CS4271_DAC_VOL_ATAPI(9);
	regLocal[CS4271_DAC_CHA_VOL] = 0x00;
	regLocal[CS4271_DAC_CHB_VOL] = 0x00;
	regLocal[CS4271_ADC_CTRL] = 0x00;
	regLocal[CS4271_MODE_CTRL2] = 0x00;
}


	
	