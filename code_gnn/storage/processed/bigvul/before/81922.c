int wc_ecc_shared_secret_ex(ecc_key* private_key, ecc_point* point,
                            byte* out, word32 *outlen)
{
    int err;

    if (private_key == NULL || point == NULL || out == NULL ||
                                                            outlen == NULL) {
        return BAD_FUNC_ARG;
    }

     
    if (private_key->type != ECC_PRIVATEKEY &&
            private_key->type != ECC_PRIVATEKEY_ONLY) {
        return ECC_BAD_ARG_E;
    }

     
    if (wc_ecc_is_valid_idx(private_key->idx) == 0)
        return ECC_BAD_ARG_E;

    switch(private_key->state) {
        case ECC_STATE_NONE:
        case ECC_STATE_SHARED_SEC_GEN:
            private_key->state = ECC_STATE_SHARED_SEC_GEN;

            err = wc_ecc_shared_secret_gen(private_key, point, out, outlen);
            if (err < 0) {
                break;
            }
            FALL_THROUGH;

        case ECC_STATE_SHARED_SEC_RES:
            private_key->state = ECC_STATE_SHARED_SEC_RES;
        #if defined(WOLFSSL_ASYNC_CRYPT) && defined(WC_ASYNC_ENABLE_ECC)
            if (private_key->asyncDev.marker == WOLFSSL_ASYNC_MARKER_ECC) {
            #ifdef HAVE_CAVIUM_V
                 
                if (NitroxEccIsCurveSupported(private_key)) {
                     
                    *outlen = private_key->dp->size;
                    XMEMCPY(out, private_key->e->raw.buf, *outlen);
                }
            #endif  
            }
        #endif  
            err = 0;
            break;

        default:
            err = BAD_STATE_E;
    }  

     
    if (err == WC_PENDING_E) {
        private_key->state++;
        return err;
    }

     
#ifdef WOLFSSL_ASYNC_CRYPT
    wc_ecc_free_async(private_key);
#endif
    private_key->state = ECC_STATE_NONE;

    return err;
}
