extern "C" int EffectGetDescriptor(const effect_uuid_t *uuid,
 effect_descriptor_t *pDescriptor) {
 int i;
 int length = sizeof(gDescriptors) / sizeof(const effect_descriptor_t *);

 if (pDescriptor == NULL || uuid == NULL){
        ALOGV("EffectGetDescriptor() called with NULL pointer");
 return -EINVAL;
 }

 for (i = 0; i < length; i++) {
 if (memcmp(uuid, &gDescriptors[i]->uuid, sizeof(effect_uuid_t)) == 0) {
 *pDescriptor = *gDescriptors[i];
            ALOGV("EffectGetDescriptor - UUID matched Reverb type %d, UUID = %x",
                 i, gDescriptors[i]->uuid.timeLow);
 return 0;
 }
 }

 return -EINVAL;
}  
