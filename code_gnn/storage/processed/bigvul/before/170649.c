const effect_descriptor_t *PreProc_GetDescriptor(const effect_uuid_t *uuid)
{
 size_t i;
 for (i = 0; i < PREPROC_NUM_EFFECTS; i++) {
 if (memcmp(&sDescriptors[i]->uuid, uuid, sizeof(effect_uuid_t)) == 0) {
 return sDescriptors[i];
 }
 }
 return NULL;
}
