int32_t DownmixLib_GetDescriptor(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor) {
    ALOGV("DownmixLib_GetDescriptor()");
 int i;

 if (pDescriptor == NULL || uuid == NULL){
        ALOGE("DownmixLib_Create() called with NULL pointer");
 return -EINVAL;
 }
    ALOGV("DownmixLib_GetDescriptor() nb effects=%d", kNbEffects);
 for (i = 0; i < kNbEffects; i++) {
        ALOGV("DownmixLib_GetDescriptor() i=%d", i);
 if (memcmp(uuid, &gDescriptors[i]->uuid, sizeof(effect_uuid_t)) == 0) {
            memcpy(pDescriptor, gDescriptors[i], sizeof(effect_descriptor_t));
            ALOGV("EffectGetDescriptor - UUID matched downmix type %d, UUID = %" PRIx32,
                 i, gDescriptors[i]->uuid.timeLow);
 return 0;
 }
 }

 return -EINVAL;
}
