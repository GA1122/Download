gsicc_profile_serialize(gsicc_serialized_profile_t *profile_data,
                        cmm_profile_t *icc_profile)
{
    if (icc_profile == NULL)
        return;
    memcpy(profile_data, icc_profile, GSICC_SERIALIZED_SIZE);
}
