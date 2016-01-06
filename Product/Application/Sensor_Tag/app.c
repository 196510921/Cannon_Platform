
#include "app.h"
/*start adv*/

static void sensor_read(void* arg);

#ifdef CANNON_V2
const char *name = "CANNON_V2";
uint8_t adv_address[] = {0x0D, 0x05, 0x04, 0x03, 0x02, 0x02};	
#endif
#ifdef CANNON_V1
const char *name = "CANNON_V1";
uint8_t adv_address[] = {0x0C, 0x05, 0x04, 0x03, 0x02, 0x01};	
#endif
uint8_t tx_power_level = 7;
uint16_t adv_interval = 48;

void jsensor_app_setSensors(void)
{
	jsensor_app_setSensor(JSENSOR_TYPE_HUMITY_TEMP);
	jsensor_app_setSensor(JSENSOR_TYPE_PRESSURE);
	jsensor_app_setSensor(JSENSOR_TYPE_MOTION_6AXIS);
	jsensor_app_setSensor(JSENSOR_TYPE_MAGNET);
}
	
void on_ready(void)
{
	/*Config Adv Parameter And Ready to Adv*/
	ble_set_adv_param((char *)name, adv_address, tx_power_level, adv_interval);
	ble_device_start_advertising();

	run_after_delay(sensor_read, NULL, 2000);
}

static void sensor_read(void* arg)
{
	// sensor read temperature & humidity
	{
		int16_t humidity;
		int16_t temperature;
		JSensor_HUM_TEMP_Typedef tdef;
	
		tdef.humidity = &humidity;
		tdef.temperature = &temperature;
	
		if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_HUMITY_TEMP, (void *)&tdef)) {
			ble_device_send(0x00, 2, (uint8_t *)&temperature);
			ble_device_send(0x01, 2, (uint8_t *)&humidity);
		}
	}
	// sensor read pressure
	{
		JSensor_Press_Typedef tdef;
		int32_t pressure;
		
		tdef.pressure = &pressure;
		
		if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_PRESSURE, (void *)&tdef)) {
			ble_device_send(0x02, 3, (uint8_t *)&pressure);
		}
	}
	// sensor read magenetometer 
	{
		JSensor_MAG_Typedef tdef;
		int8_t  MAG[6];
		tdef.MAG = MAG;
		
		if(JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_MAGNET, (void *)&tdef)) {
			ble_device_send(0x03, 6, (uint8_t*)MAG);
      //printf("%x,%x,%x,%x,%x,%x\n\r", MAG[0],MAG[1],MAG[2],MAG[3],MAG[4],MAG[5]);
		}
	}
	// sensor read motion 6 AXIS
	{
		JSensor_AXIS_Typedef tdef;
		int8_t ACC[6], GRO[6];
		tdef.ACC = ACC;
		tdef.GRO = GRO;
		
		if (JSENSOR_OK == jsensor_app_read_sensor(JSENSOR_TYPE_MOTION_6AXIS, (void *)&tdef)) {
			ble_device_send(0x04, 6, (uint8_t*)ACC);
			ble_device_send(0x05, 6, (uint8_t*)GRO);
		}
	}
	
	run_after_delay(sensor_read, NULL, 2000);
}

/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{
	
	if(type == 0x00){
		if(*value == 0x00){
			BSP_LED_On(LED0);
		}
		if(*value == 0x01){
			BSP_LED_Off(LED0);
			
		}
	}
			
}
/* Device on connect */
void ble_device_on_connect(void)
{
		
	tBleStatus ret = BLE_WAIT_REMOTE_ENABLE_NOTIFY;
	
}
/* Device on disconnect */
void ble_device_on_disconnect(uint8_t reason)
{
	/* Make the device connectable again. */
	Ble_conn_state = BLE_CONNECTABLE;
	ble_device_start_advertising();
}
