gsicc_setrange_lab(cmm_profile_t *profile)
{
    profile->Range.ranges[0].rmin = 0.0;
    profile->Range.ranges[0].rmax = 100.0;
    profile->Range.ranges[1].rmin = -128.0;
    profile->Range.ranges[1].rmax = 127.0;
    profile->Range.ranges[2].rmin = -128.0;
    profile->Range.ranges[2].rmax = 127.0;
}
