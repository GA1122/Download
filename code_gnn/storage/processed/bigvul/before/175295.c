static bool is3gpp2(int radioTech) {
 switch (radioTech) {
 case RADIO_TECH_IS95A:
 case RADIO_TECH_IS95B:
 case RADIO_TECH_1xRTT:
 case RADIO_TECH_EVDO_0:
 case RADIO_TECH_EVDO_A:
 case RADIO_TECH_EVDO_B:
 case RADIO_TECH_EHRPD:
 return true;
 default:
 return false;
 }
}
