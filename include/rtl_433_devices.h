#ifndef INCLUDE_RTL_433_DEVICES_H_
#define INCLUDE_RTL_433_DEVICES_H_

#include "bitbuffer.h"

#define DEVICES \
		DECL(silvercrest) \
		DECL(rubicson) \
		DECL(prologue) \
		DECL(waveman) \
		DECL(steffen) \
		DECL(elv_em1000) \
		DECL(elv_ws2000) \
		DECL(lacrossetx) \
		DECL(template) \
		DECL(acurite_rain_gauge) \
		DECL(acurite_th) \
		DECL(oregon_scientific) \
		DECL(mebus433) \
		DECL(intertechno) \
		DECL(newkaku) \
		DECL(alectov1) \
		DECL(cardin) \
		DECL(fineoffset_WH2) \
		DECL(nexus) \
		DECL(ambient_weather) \
		DECL(calibeur_RF104) \
		DECL(X10_RF) \
		DECL(DSC) \
		DECL(brennenstuhl_rcs_2044) \
		DECL(gt_wt_02) \
		DECL(danfoss_CFR) \
		DECL(ec3k) \
		DECL(valeo) \
		DECL(chuango) \
		DECL(generic_remote) \
		DECL(tfa_twin_plus_303049) \
		DECL(fineoffset_wh1080) \
		DECL(wt450) \
		DECL(lacrossews) \
		DECL(esperanza_ews) \
		DECL(efergy_e2_classic) \
		DECL(kw9015b) \
		DECL(generic_temperature_sensor) \
		DECL(acurite_txr) \
		DECL(acurite_986) \
		DECL(hideki_ts04) \
		DECL(oil_watchman) \
		DECL(current_cost) \
		DECL(emontx) \
		DECL(ht680) \
		DECL(s3318p) \
		DECL(akhan_100F14) \
		DECL(quhwa) \
		DECL(oregon_scientific_v1) \
		DECL(proove) \
		DECL(bresser_3ch) \
		DECL(springfield) \
		DECL(oregon_scientific_sl109h) \
		DECL(acurite_606) \
		DECL(tfa_pool_thermometer) \
		DECL(kedsum) \
		DECL(blyss) \
		DECL(steelmate) \
		DECL(schraeder) \
		DECL(lightwave_rf) \
		DECL(elro_db286a) \
		DECL(efergy_optical) \
		DECL(hondaremote) \
		DECL(template) \
		DECL(fineoffset_XC0400) \
		DECL(radiohead_ask) \
		DECL(kerui) \
		DECL(fineoffset_wh1050) \
		DECL(honeywell) \
		DECL(maverick_et73x) \
		DECL(rftech) \
		DECL(lacrosse_TX141TH_Bv2) \
		DECL(test) \
		DECL(acurite_00275rm)


typedef struct {
	char name[256];
	unsigned int modulation;
	float short_limit;
	float long_limit;
	float reset_limit;
	int (*json_callback)(bitbuffer_t *bitbuffer);
	unsigned int disabled;
	uintptr_t demod_arg;	// Decoder specific optional argument (may be pointer to struct)
	char **fields;			// List of fields this decoder produces; required for CSV output. NULL-terminated.
} r_device;

#define DECL(name) extern r_device name;
DEVICES
#undef DECL

#endif /* INCLUDE_RTL_433_DEVICES_H_ */
