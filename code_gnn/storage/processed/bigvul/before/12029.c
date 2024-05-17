int RAND_DRBG_uninstantiate(RAND_DRBG *drbg)
{
    if (drbg->meth == NULL) {
        drbg->state = DRBG_ERROR;
        RANDerr(RAND_F_RAND_DRBG_UNINSTANTIATE,
                RAND_R_NO_DRBG_IMPLEMENTATION_SELECTED);
        return 0;
    }

     
    drbg->meth->uninstantiate(drbg);
    return RAND_DRBG_set(drbg, drbg->type, drbg->flags);
}
