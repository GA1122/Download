ECPKPARAMETERS *ec_asn1_group2pkparameters(const EC_GROUP *group,
                                           ECPKPARAMETERS *params)
{
    int ok = 1, tmp;
    ECPKPARAMETERS *ret = params;

    if (ret == NULL) {
        if ((ret = ECPKPARAMETERS_new()) == NULL) {
            ECerr(EC_F_EC_ASN1_GROUP2PKPARAMETERS, ERR_R_MALLOC_FAILURE);
            return NULL;
        }
    } else {
        if (ret->type == 0 && ret->value.named_curve)
            ASN1_OBJECT_free(ret->value.named_curve);
        else if (ret->type == 1 && ret->value.parameters)
            ECPARAMETERS_free(ret->value.parameters);
    }

    if (EC_GROUP_get_asn1_flag(group)) {
         
        tmp = EC_GROUP_get_curve_name(group);
        if (tmp) {
            ret->type = 0;
            if ((ret->value.named_curve = OBJ_nid2obj(tmp)) == NULL)
                ok = 0;
        } else
             
            ok = 0;
    } else {
         
        ret->type = 1;
        if ((ret->value.parameters =
             ec_asn1_group2parameters(group, NULL)) == NULL)
            ok = 0;
    }

    if (!ok) {
        ECPKPARAMETERS_free(ret);
        return NULL;
    }
    return ret;
}
