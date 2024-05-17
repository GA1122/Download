int EC_GROUP_get_pentanomial_basis(const EC_GROUP *group, unsigned int *k1,
                                   unsigned int *k2, unsigned int *k3)
{
    if (group == NULL)
        return 0;

    if (EC_GROUP_method_of(group)->group_set_curve !=
        ec_GF2m_simple_group_set_curve || !((group->poly[0] != 0)
                                            && (group->poly[1] != 0)
                                            && (group->poly[2] != 0)
                                            && (group->poly[3] != 0)
                                            && (group->poly[4] == 0))) {
        ECerr(EC_F_EC_GROUP_GET_PENTANOMIAL_BASIS,
              ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED);
        return 0;
    }

    if (k1)
        *k1 = group->poly[3];
    if (k2)
        *k2 = group->poly[2];
    if (k3)
        *k3 = group->poly[1];

    return 1;
}
