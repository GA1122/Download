status_t ATSParser::parseTS(ABitReader *br, SyncEvent *event) {
    ALOGV("---");

 unsigned sync_byte = br->getBits(8);
 if (sync_byte != 0x47u) {
        ALOGE("[error] parseTS: return error as sync_byte=0x%x", sync_byte);
 return BAD_VALUE;
 }

 if (br->getBits(1)) {  
 return OK;
 }

 unsigned payload_unit_start_indicator = br->getBits(1);
    ALOGV("payload_unit_start_indicator = %u", payload_unit_start_indicator);

    MY_LOGV("transport_priority = %u", br->getBits(1));

 unsigned PID = br->getBits(13);
    ALOGV("PID = 0x%04x", PID);

    MY_LOGV("transport_scrambling_control = %u", br->getBits(2));

 unsigned adaptation_field_control = br->getBits(2);
    ALOGV("adaptation_field_control = %u", adaptation_field_control);

 unsigned continuity_counter = br->getBits(4);
    ALOGV("PID = 0x%04x, continuity_counter = %u", PID, continuity_counter);


 status_t err = OK;

 if (adaptation_field_control == 2 || adaptation_field_control == 3) {
        err = parseAdaptationField(br, PID);
 }
 if (err == OK) {
 if (adaptation_field_control == 1 || adaptation_field_control == 3) {
            err = parsePID(br, PID, continuity_counter,
                    payload_unit_start_indicator, event);
 }
 }

 ++mNumTSPacketsParsed;

 return err;
}
