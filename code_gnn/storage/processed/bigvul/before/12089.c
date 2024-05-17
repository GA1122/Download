void EC_GROUP_clear_free(EC_GROUP *group)
{
    if (!group)
        return;

    if (group->meth->group_clear_finish != 0)
        group->meth->group_clear_finish(group);
    else if (group->meth->group_finish != 0)
        group->meth->group_finish(group);

    EC_pre_comp_free(group);
    BN_MONT_CTX_free(group->mont_data);
    EC_POINT_clear_free(group->generator);
    BN_clear_free(group->order);
    BN_clear_free(group->cofactor);
    OPENSSL_clear_free(group->seed, group->seed_len);
    OPENSSL_clear_free(group, sizeof(*group));
}
