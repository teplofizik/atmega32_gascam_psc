#ifndef CONFIG_H_
#define CONFIG_H_

	#define I2C_ADDRESS 0x50

	#define VREF_SELECT ((1 << REFS1) | (1 << REFS0))
	#define VREF_MV     1100
	#define ADC_PRESCALER ((1 << ADPS2) | (1 << ADPS0))

	#define PSC_INPUT_ADC_CH   0
	
	#define PSC_OVP_THRESHOLD  6000
	#define PSC_UVP_THRESHOLD  3000

#endif
