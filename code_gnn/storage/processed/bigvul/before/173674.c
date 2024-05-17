status_t ATSParser::Program::parseProgramMap(ABitReader *br) {
 unsigned table_id = br->getBits(8);
    ALOGV("  table_id = %u", table_id);
 if (table_id != 0x02u) {
        ALOGE("PMT data error!");
 return ERROR_MALFORMED;
 }
 unsigned section_syntax_indicator = br->getBits(1);
    ALOGV("  section_syntax_indicator = %u", section_syntax_indicator);
 if (section_syntax_indicator != 1u) {
        ALOGE("PMT data error!");
 return ERROR_MALFORMED;
 }

    br->skipBits(1);  
    MY_LOGV("  reserved = %u", br->getBits(2));

 unsigned section_length = br->getBits(12);
    ALOGV("  section_length = %u", section_length);

    MY_LOGV("  program_number = %u", br->getBits(16));
    MY_LOGV("  reserved = %u", br->getBits(2));
    MY_LOGV("  version_number = %u", br->getBits(5));
    MY_LOGV("  current_next_indicator = %u", br->getBits(1));
    MY_LOGV("  section_number = %u", br->getBits(8));
    MY_LOGV("  last_section_number = %u", br->getBits(8));
    MY_LOGV("  reserved = %u", br->getBits(3));

 unsigned PCR_PID = br->getBits(13);
    ALOGV("  PCR_PID = 0x%04x", PCR_PID);

    MY_LOGV("  reserved = %u", br->getBits(4));

 unsigned program_info_length = br->getBits(12);
    ALOGV("  program_info_length = %u", program_info_length);

    br->skipBits(program_info_length * 8);  

 Vector<StreamInfo> infos;

 size_t infoBytesRemaining = section_length - 9 - program_info_length - 4;

 while (infoBytesRemaining >= 5) {

 unsigned streamType = br->getBits(8);
        ALOGV("    stream_type = 0x%02x", streamType);

        MY_LOGV("    reserved = %u", br->getBits(3));

 unsigned elementaryPID = br->getBits(13);
        ALOGV("    elementary_PID = 0x%04x", elementaryPID);

        MY_LOGV("    reserved = %u", br->getBits(4));

 unsigned ES_info_length = br->getBits(12);
        ALOGV("    ES_info_length = %u", ES_info_length);

#if 0
        br->skipBits(ES_info_length * 8);  
#else
 unsigned info_bytes_remaining = ES_info_length;
 while (info_bytes_remaining >= 2) {
            MY_LOGV("      tag = 0x%02x", br->getBits(8));

 unsigned descLength = br->getBits(8);
            ALOGV("      len = %u", descLength);

 if (info_bytes_remaining < descLength) {
 return ERROR_MALFORMED;
 }
            br->skipBits(descLength * 8);

            info_bytes_remaining -= descLength + 2;
 }
#endif

 StreamInfo info;
        info.mType = streamType;
        info.mPID = elementaryPID;
        infos.push(info);

        infoBytesRemaining -= 5 + ES_info_length;
 }

 if (infoBytesRemaining != 0) {
        ALOGW("Section data remains unconsumed");
 }
    MY_LOGV("  CRC = 0x%08x", br->getBits(32));

 bool PIDsChanged = false;
 for (size_t i = 0; i < infos.size(); ++i) {
 StreamInfo &info = infos.editItemAt(i);

 ssize_t index = mStreams.indexOfKey(info.mPID);

 if (index >= 0 && mStreams.editValueAt(index)->type() != info.mType) {
            ALOGI("uh oh. stream PIDs have changed.");
 PIDsChanged = true;
 break;
 }
 }

 if (PIDsChanged) {
#if 0
        ALOGI("before:");
 for (size_t i = 0; i < mStreams.size(); ++i) {
            sp<Stream> stream = mStreams.editValueAt(i);

            ALOGI("PID 0x%08x => type 0x%02x", stream->pid(), stream->type());
 }

        ALOGI("after:");
 for (size_t i = 0; i < infos.size(); ++i) {
 StreamInfo &info = infos.editItemAt(i);

            ALOGI("PID 0x%08x => type 0x%02x", info.mPID, info.mType);
 }
#endif

 bool success = switchPIDs(infos);

 if (!success) {
            ALOGI("Stream PIDs changed and we cannot recover.");
 return ERROR_MALFORMED;
 }
 }

 for (size_t i = 0; i < infos.size(); ++i) {
 StreamInfo &info = infos.editItemAt(i);

 ssize_t index = mStreams.indexOfKey(info.mPID);

 if (index < 0) {
            sp<Stream> stream = new Stream(
 this, info.mPID, info.mType, PCR_PID);

            mStreams.add(info.mPID, stream);
 }
 }

 return OK;
}
