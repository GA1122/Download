void AudioFlinger::EffectModule::process()
{
 Mutex::Autolock _l(mLock);

 if (mState == DESTROYED || mEffectInterface == NULL ||
            mConfig.inputCfg.buffer.raw == NULL ||
            mConfig.outputCfg.buffer.raw == NULL) {
 return;
 }

 if (isProcessEnabled()) {
 if ((mDescriptor.flags & EFFECT_FLAG_TYPE_MASK) == EFFECT_FLAG_TYPE_AUXILIARY) {
            ditherAndClamp(mConfig.inputCfg.buffer.s32,
                                        mConfig.inputCfg.buffer.s32,
                                        mConfig.inputCfg.buffer.frameCount/2);
 }

 int ret = (*mEffectInterface)->process(mEffectInterface,
 &mConfig.inputCfg.buffer,
 &mConfig.outputCfg.buffer);

 if (mState == STOPPED && ret == -ENODATA) {
            mDisableWaitCnt = 1;
 }

 if ((mDescriptor.flags & EFFECT_FLAG_TYPE_MASK) == EFFECT_FLAG_TYPE_AUXILIARY) {
            memset(mConfig.inputCfg.buffer.raw, 0,
                   mConfig.inputCfg.buffer.frameCount*sizeof(int32_t));
 }
 } else if ((mDescriptor.flags & EFFECT_FLAG_TYPE_MASK) == EFFECT_FLAG_TYPE_INSERT &&
                mConfig.inputCfg.buffer.raw != mConfig.outputCfg.buffer.raw) {
        sp<EffectChain> chain = mChain.promote();
 if (chain != 0 && chain->activeTrackCnt() != 0) {
 size_t frameCnt = mConfig.inputCfg.buffer.frameCount * 2;  
 int16_t *in = mConfig.inputCfg.buffer.s16;
 int16_t *out = mConfig.outputCfg.buffer.s16;
 for (size_t i = 0; i < frameCnt; i++) {
                out[i] = clamp16((int32_t)out[i] + (int32_t)in[i]);
 }
 }
 }
}
