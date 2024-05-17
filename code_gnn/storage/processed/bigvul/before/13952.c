gsicc_check_device_link(cmm_profile_t *icc_profile)
{
    bool value;

    value = gscms_is_device_link(icc_profile->profile_handle);
    icc_profile->isdevlink = value;

    return value;
}
