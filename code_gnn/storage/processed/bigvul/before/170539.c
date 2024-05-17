bool Downmix_foldGeneric(
 uint32_t mask, int16_t *pSrc, int16_t*pDst, size_t numFrames, bool accumulate) {
 if (mask & kUnsupported) {
        ALOGE("Unsupported channels (top or front left/right of center)");
 return false;
 }
 if ((mask & AUDIO_CHANNEL_OUT_STEREO) != AUDIO_CHANNEL_OUT_STEREO) {
        ALOGE("Front channels must be present");
 return false;
 }
 bool hasSides = false;
 if ((mask & kSides) != 0) {
 if ((mask & kSides) != kSides) {
            ALOGE("Side channels must be used as a pair");
 return false;
 }
        hasSides = true;
 }
 bool hasBacks = false;
 if ((mask & kBacks) != 0) {
 if ((mask & kBacks) != kBacks) {
            ALOGE("Back channels must be used as a pair");
 return false;
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

 int32_t lt, rt, centersLfeContrib;  
 if (accumulate) {
 while (numFrames) {
            centersLfeContrib = 0;
 if (hasFC) { centersLfeContrib += pSrc[indexFC]; }
 if (hasLFE) { centersLfeContrib += pSrc[indexLFE]; }
 if (hasBC) { centersLfeContrib += pSrc[indexBC]; }
            centersLfeContrib *= MINUS_3_DB_IN_Q19_12;
            lt = (pSrc[0] << 12);
            rt = (pSrc[1] << 12);
 if (hasSides) {
                lt += pSrc[indexSL] << 12;
                rt += pSrc[indexSR] << 12;
 }
 if (hasBacks) {
                lt += pSrc[indexBL] << 12;
                rt += pSrc[indexBR] << 12;
 }
            lt += centersLfeContrib;
            rt += centersLfeContrib;
            pDst[0] = clamp16(pDst[0] + (lt >> 13));
            pDst[1] = clamp16(pDst[1] + (rt >> 13));
            pSrc += numChan;
            pDst += 2;
            numFrames--;
 }
 } else {
 while (numFrames) {
            centersLfeContrib = 0;
 if (hasFC) { centersLfeContrib += pSrc[indexFC]; }
 if (hasLFE) { centersLfeContrib += pSrc[indexLFE]; }
 if (hasBC) { centersLfeContrib += pSrc[indexBC]; }
            centersLfeContrib *= MINUS_3_DB_IN_Q19_12;
            lt = (pSrc[0] << 12);
            rt = (pSrc[1] << 12);
 if (hasSides) {
                lt += pSrc[indexSL] << 12;
                rt += pSrc[indexSR] << 12;
 }
 if (hasBacks) {
                lt += pSrc[indexBL] << 12;
                rt += pSrc[indexBR] << 12;
 }
            lt += centersLfeContrib;
            rt += centersLfeContrib;
            pDst[0] = clamp16(lt >> 13);  
            pDst[1] = clamp16(rt >> 13);  
            pSrc += numChan;
            pDst += 2;
            numFrames--;
 }
 }
 return true;
}
