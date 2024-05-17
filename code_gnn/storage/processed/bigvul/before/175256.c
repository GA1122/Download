decodeCdmaSubscriptionSource (RIL_RadioState radioState) {
 switch (radioState) {
 case RADIO_STATE_SIM_NOT_READY:
 case RADIO_STATE_SIM_LOCKED_OR_ABSENT:
 case RADIO_STATE_SIM_READY:
 case RADIO_STATE_RUIM_NOT_READY:
 case RADIO_STATE_RUIM_READY:
 case RADIO_STATE_RUIM_LOCKED_OR_ABSENT:
 return CDMA_SUBSCRIPTION_SOURCE_RUIM_SIM;

 case RADIO_STATE_NV_NOT_READY:
 case RADIO_STATE_NV_READY:
 return CDMA_SUBSCRIPTION_SOURCE_NV;

 default:
            RLOGD("decodeCdmaSubscriptionSource: Invoked with incorrect RadioState");
 return -1;
 }
}
