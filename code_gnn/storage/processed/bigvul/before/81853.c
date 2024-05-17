int wc_ecc_copy_point(ecc_point* p, ecc_point *r)
{
    int ret;

     
    if (p == NULL || r == NULL)
        return ECC_BAD_ARG_E;

    ret = mp_copy(p->x, r->x);
    if (ret != MP_OKAY)
        return ret;
    ret = mp_copy(p->y, r->y);
    if (ret != MP_OKAY)
        return ret;
    ret = mp_copy(p->z, r->z);
    if (ret != MP_OKAY)
        return ret;

    return MP_OKAY;
}