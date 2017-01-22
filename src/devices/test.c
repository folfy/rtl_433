/* Nexus sensor protocol with ID, temperature and optional humidity
 * Auriol IAN-60107 (LIDL Brand)
 * also FreeTec NC-7345 sensors for FreeTec Weatherstation NC-7344.
 *
 * the sensor sends 36 bits 12 times,
 * the packets are ppm modulated (distance coding) with a pulse of ~500 us
 * followed by a short gap of ~1000 us for a 0 bit or a long ~2000 us gap for a
 * 1 bit, the sync gap is ~4000 us.
 *
 * the data is grouped in 9 nibbles
 * [id0] [id1] [flags] [temp0] [temp1] [temp2] [const] [humi0] [humi1]
 *
 * The 8-bit id changes when the battery is changed in the sensor.
 * flags are 4 bits B 0 C C, where B is the battery status: 1=OK, 0=LOW
 * and CC is the channel: 0=CH1, 1=CH2, 2=CH3
 * temp is 12 bit signed scaled by 10
 * const is always 1111 (0x0F)
 * humiditiy is 8 bits
 *
 * The sensor can be bought at Clas Ohlsen
 */

#include "rtl_433.h"
#include "util.h"
#include "data.h"

static int test_callback(bitbuffer_t *bitbuffer) {
    bitrow_t *bb = bitbuffer->bb;
    data_t *data;

    char time_str[LOCAL_TIME_BUFLEN];

    if (debug_output > 0) {
       fprintf(stderr,"Possible Test: ");
       bitbuffer_print(bitbuffer);
    }

    uint8_t id;
    uint8_t battery_low;
    uint8_t channel;
    int16_t temp;
    uint8_t humidity;

    /** The nexus protocol will trigger on rubicson data, so calculate the rubicson crc and make sure
      * it doesn't match. By guesstimate it should generate a correct crc 1/255% of the times.
      * So less then 0.5% which should be acceptable.
      */
    if (bb[0][0] == 0x00 &&
        bitbuffer->bits_per_row[1] == 40) {

        /* Get time now */
        local_time_str(0, time_str);

	/* Byte 0 is some identification code (reset after removal of battery) */
	id = bb[1][0];
	/* Bit 7:4 of Byte 1 contain some kind of checksum,
	 * Bit 3 is Button_pressed, Bit 2 is batter_low and Bit 1:0 is unknown */
        uint8_t chksum = bb[1][1]&0xF0 >> 4;
        battery_low = bb[1][1]&0x04;
	

	/* Bit 1:0 of Byte 4 is the channel ID (1-3) */
        channel = bb[1][4]&0x03;

        /* Byte 2 and high nibble of Byte 3 contains 12 bits of temperature
         * The temerature is unsigend and scaled by ~18, with an offset of +1221 */
	/* */ 
        temp = (int16_t)((uint16_t)(bb[1][2] << 8) | (bb[1][3]&0xF0));
        temp = ((temp >> 4) - 1221);

        /* Low nibble of Byte 3 is the second digit for humidity (10%), High nibble of Byte 4 is the first digit (1%) */
        humidity = (uint8_t)(((bb[1][3]&0x0F)*10)+(bb[1][4]>>4));

        // Thermo/Hygro
        data = data_make("time",          "",            DATA_STRING, time_str,
                         "model",         "",            DATA_STRING, "Test Sensor",
                         "id",            "House Code",  DATA_INT, id,
                         "battery",       "Battery",     DATA_STRING, battery_low ? "LOW" : "OK",
                         "channel",       "Channel",     DATA_INT, channel,
                         "temperature_C", "Temperature", DATA_FORMAT, "%.01f C", DATA_DOUBLE, temp/18.0,
                         "humidity",      "Humidity",    DATA_FORMAT, "%u %%", DATA_INT, humidity,
                         NULL);
        data_acquired_handler(data);

        return 1;
    }
    return 0;
}

static char *output_fields[] = {
    "time",
    "model",
    "id",
    "battery",
    "channel",
    "temperature_C",
    "humidity",
    NULL
};

r_device test = {
    .name           = "Test Sensor",
    .modulation     = OOK_PULSE_PPM_RAW,
    .short_limit    = 2400,
    .long_limit     = 5000,
    .reset_limit    = 8000,
    .json_callback  = &test_callback,
    .disabled       = 0,
    .demod_arg      = 0,
    .fields         = output_fields
};
