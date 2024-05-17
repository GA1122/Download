extern "C" int EffectGetDescriptor(const effect_uuid_t *uuid,
 effect_descriptor_t *pDescriptor) {
 const effect_descriptor_t *desc = NULL;

 if (pDescriptor == NULL || uuid == NULL){
        ALOGV("EffectGetDescriptor() called with NULL pointer");
 return -EINVAL;
 }

 if (memcmp(uuid, &gBassBoostDescriptor.uuid, sizeof(effect_uuid_t)) == 0) {
        desc = &gBassBoostDescriptor;
 } else if (memcmp(uuid, &gVirtualizerDescriptor.uuid, sizeof(effect_uuid_t)) == 0) {
        desc = &gVirtualizerDescriptor;
 } else if (memcmp(uuid, &gEqualizerDescriptor.uuid, sizeof(effect_uuid_t)) == 0) {
        desc = &gEqualizerDescriptor;
 } else if (memcmp(uuid, &gVolumeDescriptor.uuid, sizeof(effect_uuid_t)) == 0) {
        desc = &gVolumeDescriptor;
 }

 if (desc == NULL) {
 return -EINVAL;
 }

 *pDescriptor = *desc;

 return 0;
}  
