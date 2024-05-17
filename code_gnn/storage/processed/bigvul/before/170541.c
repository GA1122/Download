void Downmix_testIndexComputation(uint32_t mask) {
    ALOGI("Testing index computation for 0x%" PRIx32 ":", mask);
 if (mask & kUnsupported) {
        ALOGE("Unsupported channels (top or front left/right of center)");
 return;
 }
 if ((mask & AUDIO_CHANNEL_OUT_STEREO) != AUDIO_CHANNEL_OUT_STEREO) {
        ALOGE("Front channels must be present");
 return;
 }
 bool hasSides = false;
 if ((mask & kSides) != 0) {
 if ((mask & kSides) != kSides) {
            ALOGE("Side channels must be used as a pair");
 return;
 }
        hasSides = true;
 }
 bool hasBacks = false;
 if ((mask & kBacks) != 0) {
 if ((mask & kBacks) != kBacks) {
            ALOGE("Back channels must be used as a pair");
 return;
 }
        hasBacks = true;
 }

 const int numChan = audio_channel_count_from_out_mask(mask);
 const bool hasFC = ((mask & AUDIO_CHANNEL_OUT_FRONT_CENTER) == AUDIO_CHANNEL_OUT_FRONT_CENTER);
 const bool hasLFE =
 ((mask & AUDIO_CHANNEL_OUT_LOW_FREQUENCY) == AUDIO_CHANNEL_OUT_LOW_FREQUENCY);
 const bool hasBC = ((mask & AUDIO_CHANNEL_OUT_BACK_CENTER) == AUDIO_CHANNEL_OUT_BACK_CENTER);
 const int indexFC  = hasFC    ? 2 : 1;  
 const int indexLFE = hasLFE   ? indexFC + 1 : indexFC;  
 const int indexBL  = hasBacks ? indexLFE + 1 : indexLFE;  
 const int indexBR  = hasBacks ? indexBL + 1 : indexBL;  
 const int indexBC  = hasBC    ? indexBR + 1 : indexBR;  
 const int indexSL  = hasSides ? indexBC + 1 : indexBC;  
 const int indexSR  = hasSides ? indexSL + 1 : indexSL;  

    ALOGI("  FL FR FC LFE BL BR BC SL SR");
    ALOGI("   %d  %d  %d   %d  %d  %d  %d  %d  %d",
 0, 1, indexFC, indexLFE, indexBL, indexBR, indexBC, indexSL, indexSR);
}
