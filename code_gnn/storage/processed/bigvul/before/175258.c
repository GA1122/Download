decodeVoiceRadioTechnology (RIL_RadioState radioState) {
 switch (radioState) {
 case RADIO_STATE_SIM_NOT_READY:
 case RADIO_STATE_SIM_LOCKED_OR_ABSENT:
 case RADIO_STATE_SIM_READY:
 return RADIO_TECH_UMTS;

 case RADIO_STATE_RUIM_NOT_READY:
 case RADIO_STATE_RUIM_READY:
 case RADIO_STATE_RUIM_LOCKED_OR_ABSENT:
 case RADIO_STATE_NV_NOT_READY:
 case RADIO_STATE_NV_READY:
 return RADIO_TECH_1xRTT;

 default:
            RLOGD("decodeVoiceRadioTechnology: Invoked with incorrect RadioState");
 return -1;
 }
}
