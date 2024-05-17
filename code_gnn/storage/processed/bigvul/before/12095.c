void EC_pre_comp_free(EC_GROUP *group)
{
    switch (group->pre_comp_type) {
    default:
        break;
#ifdef ECP_NISTZ256_REFERENCE_IMPLEMENTATION
    case PCT_nistz256:
        EC_nistz256_pre_comp_free(group->pre_comp.nistz256);
        break;
#endif
#ifndef OPENSSL_NO_EC_NISTP_64_GCC_128
    case PCT_nistp224:
        EC_nistp224_pre_comp_free(group->pre_comp.nistp224);
        break;
    case PCT_nistp256:
        EC_nistp256_pre_comp_free(group->pre_comp.nistp256);
        break;
    case PCT_nistp521:
        EC_nistp521_pre_comp_free(group->pre_comp.nistp521);
        break;
#endif
    case PCT_ec:
        EC_ec_pre_comp_free(group->pre_comp.ec);
        break;
    }
    group->pre_comp.ec = NULL;
}
