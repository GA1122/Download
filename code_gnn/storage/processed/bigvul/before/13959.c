char* gsicc_get_dev_icccolorants(cmm_dev_profile_t *dev_profile)
{
    if (dev_profile == NULL || dev_profile->spotnames == NULL ||
        dev_profile->spotnames->name_str == NULL)
        return 0;
    else
        return dev_profile->spotnames->name_str;
}
