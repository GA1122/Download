int PreProcessingLib_GetDescriptor(const effect_uuid_t *uuid,
 effect_descriptor_t *pDescriptor) {

 if (pDescriptor == NULL || uuid == NULL){
 return -EINVAL;
 }

 const effect_descriptor_t *desc = PreProc_GetDescriptor(uuid);
 if (desc == NULL) {
        ALOGV("PreProcessingLib_GetDescriptor() not found");
 return -EINVAL;
 }

    ALOGV("PreProcessingLib_GetDescriptor() got fx %s", desc->name);

 *pDescriptor = *desc;
 return 0;
}
