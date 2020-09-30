#ifndef _SETTINGS_H_
#define _SETTINGS_H_
#include "settings.h"
#include "string.h"
#include "EEPROM_emulation.h"

int EEPROM_ReadSettings(SETTINGS *settings){
	uint16_t mem[sizeof(SETTINGS)/2];
	
	for(int ADDR=0;ADDR<sizeof(SETTINGS)/2;ADDR++){
		if(EEPROM_OK!=EEreadB(ADDR,&mem[ADDR]))return -1;
	}
	memcpy(settings,mem,sizeof(SETTINGS));
	if(settings->magic!=MAGIC_2BYTE) return -1;	
	
return 1;
}

void MakeDefaults(SETTINGS *settings){
	memset(settings,0,sizeof(SETTINGS));
	
	settings->PWM.L_coefficient=1;
	
	settings->PROTECTION.Ud_min			=200;
	settings->PROTECTION.Ud_max			=800;
	settings->PROTECTION.temperature=50;
	settings->PROTECTION.U_min			=220*0.8;
	settings->PROTECTION.U_max			=200*1.3;
	settings->PROTECTION.Fnet_min		=45;
	settings->PROTECTION.Fnet_max		=55;
	settings->PROTECTION.I_max_rms	=10;
	settings->PROTECTION.I_max_peak	=15;		
	
	settings->CAPACITORS.Ud_nominal	=750;		
	settings->CAPACITORS.Ud_precharge=250;
	
	for(int i=0;i<ADC_CHANNEL_NUMBER;i++){
		settings->CALIBRATIONS.calibration[i]=1;
	}		
}

int ReadSettings(SETTINGS *settings){
	if(EEPROM_ReadSettings(settings)<0){
		//WARNING(DEF_LOG("EEPROM Read Settings FAILED! May be corrupted or first time running\n"));
		MakeDefaults(settings);
		SaveSettings(settings);
	}
	return 1;
}

int EEPROM_WriteSettings(SETTINGS *settings){
	uint16_t DATA;
	uint16_t mem[sizeof(SETTINGS)/2];
	
	settings->magic=MAGIC_2BYTE;
	memcpy(mem,settings,sizeof(SETTINGS));
	for(int ADDR=0;ADDR<sizeof(SETTINGS)/2;ADDR++){
		DATA=mem[ADDR];
		if(EEPROM_OK!=EEwrite(ADDR,DATA))return -1;
	}
	return 1;
}
int SaveSettings(SETTINGS *settings){
	//return 0;//TODO:
	if(EEPROM_WriteSettings(settings)<0){
		//ERROR(ERROR_LOG("EEPROM Write Settings FAILED!\n"));
		return -1;
	}
	return 1;
}

#endif
