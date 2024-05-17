int Effect_getDescriptor(effect_handle_t   self,
 effect_descriptor_t *pDescriptor)
{
 EffectContext * pContext = (EffectContext *) self;
 const effect_descriptor_t *desc;

 if (pContext == NULL || pDescriptor == NULL) {
        ALOGV("Effect_getDescriptor() invalid param");
 return -EINVAL;
 }

 switch(pContext->EffectType) {
 case LVM_BASS_BOOST:
            desc = &android::gBassBoostDescriptor;
 break;
 case LVM_VIRTUALIZER:
            desc = &android::gVirtualizerDescriptor;
 break;
 case LVM_EQUALIZER:
            desc = &android::gEqualizerDescriptor;
 break;
 case LVM_VOLUME:
            desc = &android::gVolumeDescriptor;
 break;
 default:
 return -EINVAL;
 }

 *pDescriptor = *desc;

 return 0;
}  
