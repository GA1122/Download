void gscms_set_icc_range(cmm_profile_t **icc_profile)
{
    int num_comp = (*icc_profile)->num_comps;
    int k;

    for ( k = 0; k < num_comp; k++) {
        (*icc_profile)->Range.ranges[k].rmin = 0.0;
        (*icc_profile)->Range.ranges[k].rmax = 1.0;
    }
}
