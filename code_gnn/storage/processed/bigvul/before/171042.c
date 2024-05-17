AudioFlinger::EffectModule::~EffectModule()
{
    ALOGV("Destructor %p", this);
 if (mEffectInterface != NULL) {
        remove_effect_from_hal_l();
 EffectRelease(mEffectInterface);
 }
}
