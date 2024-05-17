status_t ATSParser::feedTSPacket(const void *data, size_t size,
 SyncEvent *event) {
 if (size != kTSPacketSize) {
        ALOGE("Wrong TS packet size");
 return BAD_VALUE;
 }

 ABitReader br((const uint8_t *)data, kTSPacketSize);
 return parseTS(&br, event);
}
