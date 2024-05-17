int Reverb_getDescriptor(effect_handle_t   self,
 effect_descriptor_t *pDescriptor)
{
    android::ReverbContext * pContext = (android::ReverbContext *)self;
 const effect_descriptor_t *desc;

 if (pContext == NULL || pDescriptor == NULL) {
        ALOGV("Reverb_getDescriptor() invalid param");
 return -EINVAL;
 }

 if (pContext->auxiliary) {
 if (pContext->preset) {
            desc = &android::gAuxPresetReverbDescriptor;
 } else {
            desc = &android::gAuxEnvReverbDescriptor;
 }
 } else {
 if (pContext->preset) {
            desc = &android::gInsertPresetReverbDescriptor;
 } else {
            desc = &android::gInsertEnvReverbDescriptor;
 }
 }

 *pDescriptor = *desc;

 return 0;
}  
