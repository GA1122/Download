int effect_lib_get_descriptor(const effect_uuid_t *uuid,
 effect_descriptor_t *descriptor)
{
 int i;

 if (lib_init() != 0)
 return init_status;

 if (descriptor == NULL || uuid == NULL) {
        ALOGV("%s called with NULL pointer", __func__);
 return -EINVAL;
 }

 for (i = 0; descriptors[i] != NULL; i++) {
 if (memcmp(uuid, &descriptors[i]->uuid, sizeof(effect_uuid_t)) == 0) {
 *descriptor = *descriptors[i];
 return 0;
 }
 }

 return -EINVAL;
}
