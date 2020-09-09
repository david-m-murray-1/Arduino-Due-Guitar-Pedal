#ifndef CS4271_TWI_H_
#define CS4271_TWI_H_

#include <asf.h>
#include "twi.h"
#include "I2C.h"

#define CS4271_I2C_ADDR			0x10		// See SuperAudioBoard github -- CS4272.cpp
#define CS4271_TWI_BUFFER_LEN	32
#define CS4271_ADDR 0x10 // TODO: need to double check

// Section 8.1 Mode Control
#define CS4271_MODE_CONTROL			(uint8_t)0x01
#define CS4271_MC_FUNC_MODE(x)			(uint8_t)(((x) & 0x03) << 6)
#define CS4271_MC_RATIO_SEL(x)			(uint8_t)(((x) & 0x03) << 4)
#define CS4271_MC_MASTER_SLAVE			(uint8_t)0x08
#define CS4271_MC_SERIAL_FORMAT(x)		(uint8_t)(((x) & 0x07) << 0)

// Section 8.2 DAC Control
#define CS4271_DAC_CONTROL			(uint8_t)0x02
#define CS4271_DAC_CTRL_AUTO_MUTE		(uint8_t)0x80
#define CS4271_DAC_CTRL_FILTER_SEL		(uint8_t)0x40
#define CS4271_DAC_CTRL_DE_EMPHASIS(x)		(uint8_t)(((x) & 0x03) << 4)
#define CS4271_DAC_CTRL_VOL_RAMP_UP		(uint8_t)0x08
#define CS4271_DAC_CTRL_VOL_RAMP_DN		(uint8_t)0x04
#define CS4271_DAC_CTRL_INV_POL(x)		(uint8_t)(((x) & 0x03) << 0)

// Section 8.3 DAC Volume and Mixing
#define CS4271_DAC_VOL				(uint8_t)0x03
#define CS4271_DAC_VOL_CH_VOL_TRACKING		(uint8_t)0x40
#define CS4271_DAC_VOL_SOFT_RAMP(x)		(uint8_t)(((x) & 0x03) << 4)
#define CS4271_DAC_VOL_ATAPI(x)			(uint8_t)(((x) & 0x0F) << 0)

// Section 8.4 DAC Channel A volume
#define CS4271_DAC_CHA_VOL			(uint8_t)0x04
#define CS4271_DAC_CHA_VOL_MUTE			(uint8_t)0x80
#define CS4271_DAC_CHA_VOL_VOLUME(x)		(uint8_t)(((x) & 0x7F) << 0)

// Section 8.5 DAC Channel B volume
#define CS4271_DAC_CHB_VOL			(uint8_t)0x05
#define CS4271_DAC_CHB_VOL_MUTE			(uint8_t)0x80
#define CS4271_DAC_CHB_VOL_VOLUME(x)		(uint8_t)(((x) & 0x7F) << 0)

// Section 8.6 ADC Control
#define CS4271_ADC_CTRL				(uint8_t)0x06
#define CS4271_ADC_CTRL_DITHER			(uint8_t)0x20
#define CS4271_ADC_CTRL_SER_FORMAT		(uint8_t)0x10
#define CS4271_ADC_CTRL_MUTE(x)			(uint8_t)(((x) & 0x03) << 2)
#define CS4271_ADC_CTRL_HPF(x)			(uint8_t)(((x) & 0x03) << 0)

// Section 8.7 Mode Control 2
#define CS4271_MODE_CTRL2			(uint8_t)0x07
#define CS4271_MODE_CTRL2_LOOP			(uint8_t)0x10
#define CS4271_MODE_CTRL2_MUTE_TRACK		(uint8_t)0x08
#define CS4271_MODE_CTRL2_CTRL_FREEZE		(uint8_t)0x04
#define CS4271_MODE_CTRL2_CTRL_PORT_EN		(uint8_t)0x02
#define CS4271_MODE_CTRL2_POWER_DOWN		(uint8_t)0x01

// Section 8.8 Chip ID
#define CS4271_CHIP_ID				(uint8_t)0x08
#define CS4271_CHIP_ID_PART(x)			(uint8_t)(((x) & 0x0F) << 4)
#define CS4271_CHIP_ID_REV(x)			(uint8_t)(((x) & 0x0F) << 0)


class AudioControlCS4271 {
public:
	uint8_t regLocal[8];
	void init();
	uint8_t WriteCS4271Register(uint8_t mpuRegister, uint8_t value);
	uint8_t ReadCS4271Register(uint8_t CS4271Register);
	void codec_write(uint8_t reg, uint8_t data);
	uint8_t codec_read(uint8_t reg);
	bool disableDither(void);
	bool enableDither(void);
	bool unmuteInput(void);
	bool muteInput(void);
	bool muteOutput(void);
	bool dacVolume(float left, float right);
	bool volume(float left, float right);
	bool volumeInteger(unsigned int n);
	bool enable(void);
	void SetupPeripherals(void)		;
	AudioControlCS4271() {
		SetupPeripherals();
	}
};

#endif
