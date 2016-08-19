#include <stdbool.h>
#include <stdint.h>
#include "led.h"
#include "nordic_common.h"
#include "app_timer.h"
#include "softdevice_handler.h"
#include "app_gpiote.h"
#include "nrf_drv_adc.h"
#include "app_util_platform.h"

#include "audible.h"
#include "simple_ble.h"
#include "eddystone.h"
#include "simple_adv.h"
#include "multi_adv.h"

// ADC config constants
#define ADC_BUFFER_SIZE 10                              // Size of buffer for ADC samples.
static nrf_adc_value_t adc_buffer[ADC_BUFFER_SIZE];     // ADC buffer
static nrf_drv_adc_channel_t m_channel_config
 = NRF_DRV_ADC_DEFAULT_CHANNEL(NRF_ADC_CONFIG_INPUT_7); // Use A7. Default configuration used


// How many milliseconds between switching advertisements
#define ADV_SWITCH_MS 1000

// Manufacturer specific data setup
#define UMICH_COMPANY_IDENTIFIER 0x02E0
#define AUDIBLE_SERVICE 0x1E

#define LED 18
 
// https://rawgit.com/lab11/blees/master/summon/squall-pir/index.html
#define PHYSWEB_URL "goo.gl/q6hmSJ"  //CHANGE THIS!!!

// Need this for one second timer
#define ONE_SECOND APP_TIMER_TICKS(6000, APP_TIMER_PRESCALER)

// Need this for the app_gpiote library
app_gpiote_user_id_t gpiote_user;

// Define our one-second timeout timer
APP_TIMER_DEF(one_second_timer);


typedef struct {
    uint16_t sound_level;
} __attribute__((packed)) aud_data_t;

// Audible Data
aud_data_t aud_data;

// Buffer for the manufacturer specific data
uint8_t mdata[1 + sizeof(aud_data_t)];

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x90,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = DEVICE_NAME,       // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};


static void adv_config_eddystone () {
    eddystone_adv(PHYSWEB_URL, NULL);
}

static void adv_config_data () {
    ble_advdata_manuf_data_t mandata;

    // Put in service byte
    mdata[0] = AUDIBLE_SERVICE;

    // Copy in latest data
    memcpy(mdata+1, (uint8_t*) &aud_data, sizeof(aud_data_t));

    // Fill out nordic struct
    mandata.company_identifier = UMICH_COMPANY_IDENTIFIER;
    mandata.data.p_data = mdata;
    mandata.data.size   = 1 + sizeof(aud_data_t);

    simple_adv_manuf_data(&mandata);
}

// void interrupt_handler (uint32_t pins_l2h, uint32_t pins_h2l) {

// //     if (pins_l2h & (1 << INTERRUPT_PIN)) {
// //         // The PIR interrupt pin when high.
// // #if USE_LED
// //         led_on(LED);
// // #endif

// //         // Mark that the PIN is currently high
// //         // pir_data.current_motion = 1;

// //         // We also know that we have motion since the last advertisement
// //         // and in the last minute.
// //         // pir_data.motion_since_last_adv = 1;
// //         // pir_data.motion_last_minute = 1;

// //         // Stop the one minute timer if it is started. We wait to set our
// //         // one minute timeout when motion stops.
// //         app_timer_stop(one_minute_timer);

// //     } else if (pins_h2l & (1 << INTERRUPT_PIN)) {
// //         // Motion detection stopped
// // #if USE_LED
// //         led_off(LED);
// // #endif

// //         // No more detected motion
// //         // pir_data.current_motion = 0;

// //         // Call a one-shot timer here to to set motion to low if it ever fires.
// //         app_timer_start(one_minute_timer, ONE_MINUTE, NULL);
// //     }
// }

// ADC interrupt handler.
static void adc_event_handler(nrf_drv_adc_evt_t const * p_event) {
	// When ADB buffer is full of samples
    if (p_event->type == NRF_DRV_ADC_EVT_DONE)
    {
        uint32_t i;
        for (i = 0; i < p_event->data.done.size; i++)
        {
            // Get data
            aud_data.sound_level = p_event->data.done.p_buffer[i];
            if(p_event->data.done.p_buffer[i] > 512) {
                led_on(LED);
            }else {
                led_off(LED);
            }                
        }

        // Reset Integrator
        audible_integrator_reset();
    }
}

// Called one minute after motion stopped.
void one_second_timer_handler (void* p_context) {
    
    
    APP_ERROR_CHECK(nrf_drv_adc_buffer_convert(adc_buffer,ADC_BUFFER_SIZE));
    uint32_t i;
    for (i = 0; i < ADC_BUFFER_SIZE; i++)
    {
        // manually trigger ADC conversion
        nrf_drv_adc_sample();
    }
}

static void adc_config(void) {
    ret_code_t ret_code;
    nrf_drv_adc_config_t config = NRF_DRV_ADC_DEFAULT_CONFIG;

    ret_code = nrf_drv_adc_init(&config, adc_event_handler);
    APP_ERROR_CHECK(ret_code);

    nrf_drv_adc_channel_enable(&m_channel_config);
}

int main(void) {

    // Initialize.
    // led_init(LED);
    // led_off(LED);

    // Enable internal DC-DC converter to save power
    // sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);

	// Init audiBLE
	audible_init();

    // Setup ADC
    adc_config();

    // Setup BLE
    simple_ble_init(&ble_config);

    // Need to init multi adv
    multi_adv_init(ADV_SWITCH_MS);

    // Now register our advertisement configure functions
    multi_adv_register_config(adv_config_eddystone);
    multi_adv_register_config(adv_config_data);


    // For 3 users of GPIOTE
    APP_GPIOTE_INIT(3);

    // Register us as one user
    // app_gpiote_user_register(&gpiote_user,
    //                          1<<INTERRUPT_PIN,   // Which pins we want the interrupt for low to high
    //                          1<<INTERRUPT_PIN,   // Which pins we want the interrupt for high to low
    //                          interrupt_handler);

    // Setup a timer to keep track of a minute
    app_timer_create(&one_second_timer,
                     APP_TIMER_MODE_REPEATED,
                     one_second_timer_handler);

    app_timer_start(one_second_timer, ONE_SECOND, NULL);

    // PIR pin needs a pull down
    // nrf_gpio_cfg_input(INTERRUPT_PIN, NRF_GPIO_PIN_PULLDOWN);

    // Ready to go!
    app_gpiote_user_enable(gpiote_user);

    // Start rotating
    multi_adv_start();

    // Enter main loop.
    while (1) {
        sd_app_evt_wait();
    }
}