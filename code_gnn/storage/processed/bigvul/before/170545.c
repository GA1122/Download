int LE_getDescriptor(effect_handle_t   self,
 effect_descriptor_t *pDescriptor)
{
 LoudnessEnhancerContext * pContext = (LoudnessEnhancerContext *) self;

 if (pContext == NULL || pDescriptor == NULL) {
        ALOGV("LE_getDescriptor() invalid param");
 return -EINVAL;
 }

 *pDescriptor = gLEDescriptor;

 return 0;
}  
