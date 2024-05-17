int rand_drbg_restart(RAND_DRBG *drbg,
                      const unsigned char *buffer, size_t len, size_t entropy)
{
    int reseeded = 0;
    const unsigned char *adin = NULL;
    size_t adinlen = 0;

    if (drbg->seed_pool != NULL) {
        RANDerr(RAND_F_RAND_DRBG_RESTART, ERR_R_INTERNAL_ERROR);
        drbg->state = DRBG_ERROR;
        rand_pool_free(drbg->seed_pool);
        drbg->seed_pool = NULL;
        return 0;
    }

    if (buffer != NULL) {
        if (entropy > 0) {
            if (drbg->max_entropylen < len) {
                RANDerr(RAND_F_RAND_DRBG_RESTART,
                    RAND_R_ENTROPY_INPUT_TOO_LONG);
                drbg->state = DRBG_ERROR;
                return 0;
            }

            if (entropy > 8 * len) {
                RANDerr(RAND_F_RAND_DRBG_RESTART, RAND_R_ENTROPY_OUT_OF_RANGE);
                drbg->state = DRBG_ERROR;
                return 0;
            }

             
            drbg->seed_pool = rand_pool_attach(buffer, len, entropy);
            if (drbg->seed_pool == NULL)
                return 0;
        } else {
            if (drbg->max_adinlen < len) {
                RANDerr(RAND_F_RAND_DRBG_RESTART,
                        RAND_R_ADDITIONAL_INPUT_TOO_LONG);
                drbg->state = DRBG_ERROR;
                return 0;
            }
            adin = buffer;
            adinlen = len;
        }
    }

     
    if (drbg->state == DRBG_ERROR)
        RAND_DRBG_uninstantiate(drbg);

     
    if (drbg->state == DRBG_UNINITIALISED) {
         
        RAND_DRBG_instantiate(drbg,
                              (const unsigned char *) ossl_pers_string,
                              sizeof(ossl_pers_string) - 1);
         
        reseeded = (drbg->state == DRBG_READY);
    }

     
    if (drbg->state == DRBG_READY) {
        if (adin != NULL) {
             
            drbg->meth->reseed(drbg, adin, adinlen, NULL, 0);
        } else if (reseeded == 0) {
             
            RAND_DRBG_reseed(drbg, NULL, 0, 0);
        }
    }

    rand_pool_free(drbg->seed_pool);
    drbg->seed_pool = NULL;

    return drbg->state == DRBG_READY;
}
