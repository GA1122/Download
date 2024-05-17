status_t MPEG4Extractor::updateAudioTrackInfoFromESDS_MPEG4Audio(
 const void *esds_data, size_t esds_size) {
    ESDS esds(esds_data, esds_size);

 uint8_t objectTypeIndication;
 if (esds.getObjectTypeIndication(&objectTypeIndication) != OK) {
 return ERROR_MALFORMED;
 }

 if (objectTypeIndication == 0xe1) {
 if (mLastTrack == NULL)
 return ERROR_MALFORMED;

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

 if (kUseHexDump) {
        printf("ESD of size %zu\n", csd_size);
        hexdump(csd, csd_size);
 }

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

 if (mLastTrack == NULL)
 return ERROR_MALFORMED;

    mLastTrack->meta->setInt32(kKeyAACAOT, objectType);

 uint32_t freqIndex = br.getBits(4);

 int32_t sampleRate = 0;
 int32_t numChannels = 0;
 if (freqIndex == 15) {
 if (br.numBitsLeft() < 28) return ERROR_MALFORMED;
        sampleRate = br.getBits(24);
        numChannels = br.getBits(4);
 } else {
 if (br.numBitsLeft() < 4) return ERROR_MALFORMED;
        numChannels = br.getBits(4);

 if (freqIndex == 13 || freqIndex == 14) {
 return ERROR_MALFORMED;
 }

        sampleRate = kSamplingRate[freqIndex];
 }

 if (objectType == AOT_SBR || objectType == AOT_PS) { 
 if (br.numBitsLeft() < 4) return ERROR_MALFORMED;
 uint32_t extFreqIndex = br.getBits(4);
 int32_t extSampleRate __unused;
 if (extFreqIndex == 15) {
 if (csd_size < 8) {
 return ERROR_MALFORMED;
 }
 if (br.numBitsLeft() < 24) return ERROR_MALFORMED;
            extSampleRate = br.getBits(24);
 } else {
 if (extFreqIndex == 13 || extFreqIndex == 14) {
 return ERROR_MALFORMED;
 }
            extSampleRate = kSamplingRate[extFreqIndex];
 }
 }

 switch (numChannels) {
 case 0:
 case 1: 
 case 2: 
 case 3: 
 case 4: 
 case 5: 
 case 6: 
 break;
 case 11: 
            numChannels = 7;
 break;
 case 7:  
 case 12: 
 case 14: 
            numChannels = 8;
 break;
 default:
 return ERROR_UNSUPPORTED;
 }

 {
 if (objectType == AOT_SBR || objectType == AOT_PS) {
 if (br.numBitsLeft() < 5) return ERROR_MALFORMED;
            objectType = br.getBits(5);

 if (objectType == AOT_ESCAPE) {
 if (br.numBitsLeft() < 6) return ERROR_MALFORMED;
                objectType = 32 + br.getBits(6);
 }
 }
 if (objectType == AOT_AAC_LC || objectType == AOT_ER_AAC_LC ||
                objectType == AOT_ER_AAC_LD || objectType == AOT_ER_AAC_SCAL ||
                objectType == AOT_ER_BSAC) {
 if (br.numBitsLeft() < 2) return ERROR_MALFORMED;
 const int32_t frameLengthFlag __unused = br.getBits(1);

 const int32_t dependsOnCoreCoder = br.getBits(1);

 if (dependsOnCoreCoder ) {
 if (br.numBitsLeft() < 14) return ERROR_MALFORMED;
 const int32_t coreCoderDelay __unused = br.getBits(14);
 }

 int32_t extensionFlag = -1;
 if (br.numBitsLeft() > 0) {
                extensionFlag = br.getBits(1);
 } else {
 switch (objectType) {
 case AOT_AAC_LC:
                    extensionFlag = 0;
 break;
 case AOT_ER_AAC_LC:
 case AOT_ER_AAC_SCAL:
 case AOT_ER_BSAC:
 case AOT_ER_AAC_LD:
                    extensionFlag = 1;
 break;
 default:
 return ERROR_MALFORMED;
 break;
 }
                ALOGW("csd missing extension flag; assuming %d for object type %u.",
                        extensionFlag, objectType);
 }

 if (numChannels == 0) {
 int32_t channelsEffectiveNum = 0;
 int32_t channelsNum = 0;
 if (br.numBitsLeft() < 32) {
 return ERROR_MALFORMED;
 }
 const int32_t ElementInstanceTag __unused = br.getBits(4);
 const int32_t Profile __unused = br.getBits(2);
 const int32_t SamplingFrequencyIndex __unused = br.getBits(4);
 const int32_t NumFrontChannelElements = br.getBits(4);
 const int32_t NumSideChannelElements = br.getBits(4);
 const int32_t NumBackChannelElements = br.getBits(4);
 const int32_t NumLfeChannelElements = br.getBits(2);
 const int32_t NumAssocDataElements __unused = br.getBits(3);
 const int32_t NumValidCcElements __unused = br.getBits(4);

 const int32_t MonoMixdownPresent = br.getBits(1);

 if (MonoMixdownPresent != 0) {
 if (br.numBitsLeft() < 4) return ERROR_MALFORMED;
 const int32_t MonoMixdownElementNumber __unused = br.getBits(4);
 }

 if (br.numBitsLeft() < 1) return ERROR_MALFORMED;
 const int32_t StereoMixdownPresent = br.getBits(1);
 if (StereoMixdownPresent != 0) {
 if (br.numBitsLeft() < 4) return ERROR_MALFORMED;
 const int32_t StereoMixdownElementNumber __unused = br.getBits(4);
 }

 if (br.numBitsLeft() < 1) return ERROR_MALFORMED;
 const int32_t MatrixMixdownIndexPresent = br.getBits(1);
 if (MatrixMixdownIndexPresent != 0) {
 if (br.numBitsLeft() < 3) return ERROR_MALFORMED;
 const int32_t MatrixMixdownIndex __unused = br.getBits(2);
 const int32_t PseudoSurroundEnable __unused = br.getBits(1);
 }

 int i;
 for (i=0; i < NumFrontChannelElements; i++) {
 if (br.numBitsLeft() < 5) return ERROR_MALFORMED;
 const int32_t FrontElementIsCpe = br.getBits(1);
 const int32_t FrontElementTagSelect __unused = br.getBits(4);
                    channelsNum += FrontElementIsCpe ? 2 : 1;
 }

 for (i=0; i < NumSideChannelElements; i++) {
 if (br.numBitsLeft() < 5) return ERROR_MALFORMED;
 const int32_t SideElementIsCpe = br.getBits(1);
 const int32_t SideElementTagSelect __unused = br.getBits(4);
                    channelsNum += SideElementIsCpe ? 2 : 1;
 }

 for (i=0; i < NumBackChannelElements; i++) {
 if (br.numBitsLeft() < 5) return ERROR_MALFORMED;
 const int32_t BackElementIsCpe = br.getBits(1);
 const int32_t BackElementTagSelect __unused = br.getBits(4);
                    channelsNum += BackElementIsCpe ? 2 : 1;
 }
                channelsEffectiveNum = channelsNum;

 for (i=0; i < NumLfeChannelElements; i++) {
 if (br.numBitsLeft() < 4) return ERROR_MALFORMED;
 const int32_t LfeElementTagSelect __unused = br.getBits(4);
                    channelsNum += 1;
 }
                ALOGV("mpeg4 audio channelsNum = %d", channelsNum);
                ALOGV("mpeg4 audio channelsEffectiveNum = %d", channelsEffectiveNum);
                numChannels = channelsNum;
 }
 }
 }

 if (numChannels == 0) {
 return ERROR_UNSUPPORTED;
 }

 if (mLastTrack == NULL)
 return ERROR_MALFORMED;

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
