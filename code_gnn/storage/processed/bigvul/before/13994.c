gsicc_set_device_blackptcomp(gx_device *dev, gsicc_blackptcomp_t blackptcomp,
                                gsicc_profile_types_t profile_type)
{
    int code = 0;
    cmm_dev_profile_t *profile_struct;

    if (dev->procs.get_profile == NULL) {
        profile_struct = dev->icc_struct;
    } else {
        code = dev_proc(dev, get_profile)(dev,  &profile_struct);
    }
    if (profile_struct ==  NULL)
        return 0;
    profile_struct->rendercond[profile_type].black_point_comp = blackptcomp;
    return code;
}
