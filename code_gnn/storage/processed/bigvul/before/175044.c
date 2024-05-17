status_t MPEG4Extractor::updateAudioTrackInfoFromESDS_MPEG4Audio(
 const void *esds_data, size_t esds_size) {
    ESDS esds(esds_data, esds_size);

 uint8_t objectTypeIndication;
 if (esds.getObjectTypeIndication(&objectTypeIndication) != OK) {
 return ERROR_MALFORMED;
 }

 if (objectTypeIndication == 0xe1) {
        mLastTrack->meta->setCString(kKeyMIMEType, MEDIA_MIMETYPE_AUDIO_QCELP);
 return OK;
 }

 if (objectTypeIndication  == 0x6b) {
        ALOGE("MP3 track in MP4/3GPP file is not supported");
 return ERROR_UNSUPPORTED;
 }

 const uint8_t *csd;
 size_t csd_size;
 if (esds.getCodecSpecificInfo(
 (const void **)&csd, &csd_size) != OK) {
 return ERROR_MALFORMED;
 }

#if 0
    printf("ESD of size %d\n", csd_size);
    hexdump(csd, csd_size);
#endif

 if (csd_size == 0) {

 return OK;
 }

 if (csd_size < 2) {
 return ERROR_MALFORMED;
 }

 static uint32_t kSamplingRate[] = {
 96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050,
 16000, 12000, 11025, 8000, 7350
 };

 ABitReader br(csd, csd_size);
 uint32_t objectType = br.getBits(5);

 if (objectType == 31) {  
        objectType = 32 + br.getBits(6);
 }

    mLastTrack->meta->setInt32(kKeyAACAOT, objectType);

 uint32_t freqIndex = br.getBits(4);

 int32_t sampleRate = 0;
 int32_t numChannels = 0;
 if (freqIndex == 15) {
 if (csd_size < 5) {
 return ERROR_MALFORMED;
 }
        sampleRate = br.getBits(24);
        numChannels = br.getBits(4);
 } else {
        numChannels = br.getBits(4);

 if (freqIndex == 13 || freqIndex == 14) {
 return ERROR_MALFORMED;
 }

        sampleRate = kSamplingRate[freqIndex];
 }

 if (objectType == 5 || objectType == 29) {  
 uint32_t extFreqIndex = br.getBits(4);
 int32_t extSampleRate;
 if (extFreqIndex == 15) {
 if (csd_size < 8) {
 return ERROR_MALFORMED;
 }
            extSampleRate = br.getBits(24);
 } else {
 if (extFreqIndex == 13 || extFreqIndex == 14) {
 return ERROR_MALFORMED;
 }
            extSampleRate = kSamplingRate[extFreqIndex];
 }
 }

 if (numChannels == 0) {
 return ERROR_UNSUPPORTED;
 }

 int32_t prevSampleRate;
    CHECK(mLastTrack->meta->findInt32(kKeySampleRate, &prevSampleRate));

 if (prevSampleRate != sampleRate) {
        ALOGV("mpeg4 audio sample rate different from previous setting. "
 "was: %d, now: %d", prevSampleRate, sampleRate);
 }

    mLastTrack->meta->setInt32(kKeySampleRate, sampleRate);

 int32_t prevChannelCount;
    CHECK(mLastTrack->meta->findInt32(kKeyChannelCount, &prevChannelCount));

 if (prevChannelCount != numChannels) {
        ALOGV("mpeg4 audio channel count different from previous setting. "
 "was: %d, now: %d", prevChannelCount, numChannels);
 }

    mLastTrack->meta->setInt32(kKeyChannelCount, numChannels);

 return OK;
}
