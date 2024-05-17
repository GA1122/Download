gsicc_set_device_profile_intent(gx_device *dev, gsicc_rendering_intents_t intent,
                                gsicc_profile_types_t profile_type)
{
    int code;
    cmm_dev_profile_t *profile_struct;

    if (dev->procs.get_profile == NULL) {
        profile_struct = dev->icc_struct;
    } else {
        code = dev_proc(dev, get_profile)(dev,  &profile_struct);
        if (code < 0)
            return code;
    }
    if (profile_struct ==  NULL)
        return 0;
    profile_struct->rendercond[profile_type].rendering_intent = intent;
    return 0;
}
