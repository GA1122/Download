static int Downmix_Process(effect_handle_t self,
 audio_buffer_t *inBuffer, audio_buffer_t *outBuffer) {

 downmix_object_t *pDownmixer;
 int16_t *pSrc, *pDst;
 downmix_module_t *pDwmModule = (downmix_module_t *)self;

 if (pDwmModule == NULL) {
 return -EINVAL;
 }

 if (inBuffer == NULL || inBuffer->raw == NULL ||
        outBuffer == NULL || outBuffer->raw == NULL ||
        inBuffer->frameCount != outBuffer->frameCount) {
 return -EINVAL;
 }

    pDownmixer = (downmix_object_t*) &pDwmModule->context;

 if (pDownmixer->state == DOWNMIX_STATE_UNINITIALIZED) {
        ALOGE("Downmix_Process error: trying to use an uninitialized downmixer");
 return -EINVAL;
 } else if (pDownmixer->state == DOWNMIX_STATE_INITIALIZED) {
        ALOGE("Downmix_Process error: trying to use a non-configured downmixer");
 return -ENODATA;
 }

    pSrc = inBuffer->s16;
    pDst = outBuffer->s16;
 size_t numFrames = outBuffer->frameCount;

 const bool accumulate =
 (pDwmModule->config.outputCfg.accessMode == EFFECT_BUFFER_ACCESS_ACCUMULATE);
 const uint32_t downmixInputChannelMask = pDwmModule->config.inputCfg.channels;

 switch(pDownmixer->type) {

 case DOWNMIX_TYPE_STRIP:
 if (accumulate) {
 while (numFrames) {
                  pDst[0] = clamp16(pDst[0] + pSrc[0]);
                  pDst[1] = clamp16(pDst[1] + pSrc[1]);
                  pSrc += pDownmixer->input_channel_count;
                  pDst += 2;
                  numFrames--;
 }
 } else {
 while (numFrames) {
                  pDst[0] = pSrc[0];
                  pDst[1] = pSrc[1];
                  pSrc += pDownmixer->input_channel_count;
                  pDst += 2;
                  numFrames--;
 }
 }
 break;

 case DOWNMIX_TYPE_FOLD:
#ifdef DOWNMIX_ALWAYS_USE_GENERIC_DOWNMIXER
 if (!Downmix_foldGeneric(
                  downmixInputChannelMask, pSrc, pDst, numFrames, accumulate)) {
              ALOGE("Multichannel configuration 0x%" PRIx32 " is not supported", downmixInputChannelMask);
 return -EINVAL;
 }
 break;
#endif
 switch((downmix_input_channel_mask_t)downmixInputChannelMask) {
 case CHANNEL_MASK_QUAD_BACK:
 case CHANNEL_MASK_QUAD_SIDE:
 Downmix_foldFromQuad(pSrc, pDst, numFrames, accumulate);
 break;
 case CHANNEL_MASK_5POINT1_BACK:
 case CHANNEL_MASK_5POINT1_SIDE:
 Downmix_foldFrom5Point1(pSrc, pDst, numFrames, accumulate);
 break;
 case CHANNEL_MASK_7POINT1:
 Downmix_foldFrom7Point1(pSrc, pDst, numFrames, accumulate);
 break;
 default:
 if (!Downmix_foldGeneric(
                    downmixInputChannelMask, pSrc, pDst, numFrames, accumulate)) {
                ALOGE("Multichannel configuration 0x%" PRIx32 " is not supported", downmixInputChannelMask);
 return -EINVAL;
 }
 break;
 }
 break;

 default:
 return -EINVAL;
 }

 return 0;
}
