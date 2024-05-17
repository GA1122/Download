int LELib_GetDescriptor(const effect_uuid_t *uuid,
 effect_descriptor_t *pDescriptor) {

 if (pDescriptor == NULL || uuid == NULL){
        ALOGV("LELib_GetDescriptor() called with NULL pointer");
 return -EINVAL;
 }

 if (memcmp(uuid, &gLEDescriptor.uuid, sizeof(effect_uuid_t)) == 0) {
 *pDescriptor = gLEDescriptor;
 return 0;
 }

 return -EINVAL;
}  
