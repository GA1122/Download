processRadioState(RIL_RadioState newRadioState, RIL_SOCKET_ID socket_id) {

 if((newRadioState > RADIO_STATE_UNAVAILABLE) && (newRadioState < RADIO_STATE_ON)) {
 int newVoiceRadioTech;
 int newCdmaSubscriptionSource;
 int newSimStatus;

  
        newVoiceRadioTech = decodeVoiceRadioTechnology(newRadioState);
 if(newVoiceRadioTech != voiceRadioTech) {
            voiceRadioTech = newVoiceRadioTech;
            RIL_UNSOL_RESPONSE(RIL_UNSOL_VOICE_RADIO_TECH_CHANGED,
 &voiceRadioTech, sizeof(voiceRadioTech), socket_id);
 }
 if(is3gpp2(newVoiceRadioTech)) {
            newCdmaSubscriptionSource = decodeCdmaSubscriptionSource(newRadioState);
 if(newCdmaSubscriptionSource != cdmaSubscriptionSource) {
                cdmaSubscriptionSource = newCdmaSubscriptionSource;
                RIL_UNSOL_RESPONSE(RIL_UNSOL_CDMA_SUBSCRIPTION_SOURCE_CHANGED,
 &cdmaSubscriptionSource, sizeof(cdmaSubscriptionSource), socket_id);
 }
 }
        newSimStatus = decodeSimStatus(newRadioState);
 if(newSimStatus != simRuimStatus) {
            simRuimStatus = newSimStatus;
            RIL_UNSOL_RESPONSE(RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED, NULL, 0, socket_id);
 }

  
        newRadioState = RADIO_STATE_ON;
 }

 return newRadioState;
}
