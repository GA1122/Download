AudioFlinger::EffectHandle::EffectHandle(const sp<EffectModule>& effect,
 const sp<AudioFlinger::Client>& client,
 const sp<IEffectClient>& effectClient,
 int32_t priority)
 : BnEffect(),
    mEffect(effect), mEffectClient(effectClient), mClient(client), mCblk(NULL),
    mPriority(priority), mHasControl(false), mEnabled(false), mDestroyed(false)
{
    ALOGV("constructor %p", this);

 if (client == 0) {
 return;
 }
 int bufOffset = ((sizeof(effect_param_cblk_t) - 1) / sizeof(int) + 1) * sizeof(int);
    mCblkMemory = client->heap()->allocate(EFFECT_PARAM_BUFFER_SIZE + bufOffset);
 if (mCblkMemory == 0 ||
 (mCblk = static_cast<effect_param_cblk_t *>(mCblkMemory->pointer())) == NULL) {
        ALOGE("not enough memory for Effect size=%u", EFFECT_PARAM_BUFFER_SIZE +
 sizeof(effect_param_cblk_t));
        mCblkMemory.clear();
 return;
 }
 new(mCblk) effect_param_cblk_t();
    mBuffer = (uint8_t *)mCblk + bufOffset;
}
