status_t MPEG4Extractor::parseAC3SpecificBox(
 off64_t offset, uint16_t sampleRate) {
 uint32_t size;
 const uint32_t kAC3SpecificBoxSize = 11;
 if (!mDataSource->getUInt32(offset, &size) || size < kAC3SpecificBoxSize) {
        ALOGE("MPEG4Extractor: error while reading ac-3 block: cannot read specific box size");
 return ERROR_MALFORMED;
 }

    offset += 4;
 uint32_t type;
 if (!mDataSource->getUInt32(offset, &type) || type != FOURCC('d', 'a', 'c', '3')) {
        ALOGE("MPEG4Extractor: error while reading ac-3 specific block: header not dac3");
 return ERROR_MALFORMED;
 }

    offset += 4;
 const uint32_t kAC3SpecificBoxPayloadSize = 3;
 uint8_t chunk[kAC3SpecificBoxPayloadSize];
 if (mDataSource->readAt(offset, chunk, sizeof(chunk)) != sizeof(chunk)) {
        ALOGE("MPEG4Extractor: error while reading ac-3 specific block: bitstream fields");
 return ERROR_MALFORMED;
 }

 ABitReader br(chunk, sizeof(chunk));
 static const unsigned channelCountTable[] = {2, 1, 2, 3, 3, 4, 4, 5};
 static const unsigned sampleRateTable[] = {48000, 44100, 32000};

 unsigned fscod = br.getBits(2);
 if (fscod == 3) {
        ALOGE("Incorrect fscod (3) in AC3 header");
 return ERROR_MALFORMED;
 }
 unsigned boxSampleRate = sampleRateTable[fscod];
 if (boxSampleRate != sampleRate) {
        ALOGE("sample rate mismatch: boxSampleRate = %d, sampleRate = %d",
            boxSampleRate, sampleRate);
 return ERROR_MALFORMED;
 }

 unsigned bsid = br.getBits(5);
 if (bsid > 8) {
        ALOGW("Incorrect bsid in AC3 header. Possibly E-AC-3?");
 return ERROR_MALFORMED;
 }

 unsigned bsmod __unused = br.getBits(3);

 unsigned acmod = br.getBits(3);
 unsigned lfeon = br.getBits(1);
 unsigned channelCount = channelCountTable[acmod] + lfeon;

 if (mLastTrack == NULL) {
 return ERROR_MALFORMED;
 }
    mLastTrack->meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_AC3);
    mLastTrack->meta->setInt32(kKeyChannelCount, channelCount);
    mLastTrack->meta->setInt32(kKeySampleRate, sampleRate);
 return OK;
}
