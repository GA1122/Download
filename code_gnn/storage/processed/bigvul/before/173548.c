static int lib_get_descriptor(const effect_uuid_t *uuid,
 effect_descriptor_t *pDescriptor)
{
 const effect_descriptor_t *desc;

 if (pDescriptor == NULL || uuid == NULL)
 return -EINVAL;

 if (init() != 0)
 return init_status;

    desc = get_descriptor(uuid);
 if (desc == NULL) {
        ALOGV("lib_get_descriptor() not found");
 return -EINVAL;
 }

    ALOGV("lib_get_descriptor() got fx %s", desc->name);

 *pDescriptor = *desc;
 return 0;
}
