#include "app.h"
#include "juma_sensor.h"
/*start adv*/

const char *name = "BlueNRG_IOT_2";
uint8_t adv_address[] = {0x08, 0x05, 0x04, 0x03, 0x02, 0x02};
uint8_t tx_power_level = 7;
uint16_t adv_interval = 100;
static float humidity;
static float temperature;

static void sensor_read(void* arg);

void on_ready(void)
{
    jsensor_app_set_sensor(JSENSOR_TYPE_HUMITY_TEMP);
    /*Config Adv Parameter And Ready to Adv*/
    ble_set_adv_param(name, adv_address, tx_power_level, adv_interval);
    ble_device_start_advertising();
    sensor_read(NULL);
}

static void sensor_read(void* arg)
{
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
}


/* Device On Message */
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{

    if(type == 0x00) {
        if(*value == 0x00) {
            BSP_LED_Off(LED0);
        }
        if(*value == 0x01) {
            BSP_LED_On(LED0);

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
}
