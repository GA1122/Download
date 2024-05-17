static int addr_validate_path_internal(X509_STORE_CTX *ctx,
                                       STACK_OF(X509) *chain,
                                       IPAddrBlocks *ext)
{
    IPAddrBlocks *child = NULL;
    int i, j, ret = 1;
    X509 *x;

    OPENSSL_assert(chain != NULL && sk_X509_num(chain) > 0);
    OPENSSL_assert(ctx != NULL || ext != NULL);
    OPENSSL_assert(ctx == NULL || ctx->verify_cb != NULL);

     
    if (ext != NULL) {
        i = -1;
        x = NULL;
    } else {
        i = 0;
        x = sk_X509_value(chain, i);
        OPENSSL_assert(x != NULL);
        if ((ext = x->rfc3779_addr) == NULL)
            goto done;
    }
    if (!X509v3_addr_is_canonical(ext))
        validation_err(X509_V_ERR_INVALID_EXTENSION);
    (void)sk_IPAddressFamily_set_cmp_func(ext, IPAddressFamily_cmp);
    if ((child = sk_IPAddressFamily_dup(ext)) == NULL) {
        X509V3err(X509V3_F_ADDR_VALIDATE_PATH_INTERNAL,
                  ERR_R_MALLOC_FAILURE);
        ctx->error = X509_V_ERR_OUT_OF_MEM;
        ret = 0;
        goto done;
    }

     
    for (i++; i < sk_X509_num(chain); i++) {
        x = sk_X509_value(chain, i);
        OPENSSL_assert(x != NULL);
        if (!X509v3_addr_is_canonical(x->rfc3779_addr))
            validation_err(X509_V_ERR_INVALID_EXTENSION);
        if (x->rfc3779_addr == NULL) {
            for (j = 0; j < sk_IPAddressFamily_num(child); j++) {
                IPAddressFamily *fc = sk_IPAddressFamily_value(child, j);
                if (fc->ipAddressChoice->type != IPAddressChoice_inherit) {
                    validation_err(X509_V_ERR_UNNESTED_RESOURCE);
                    break;
                }
            }
            continue;
        }
        (void)sk_IPAddressFamily_set_cmp_func(x->rfc3779_addr,
                                              IPAddressFamily_cmp);
        for (j = 0; j < sk_IPAddressFamily_num(child); j++) {
            IPAddressFamily *fc = sk_IPAddressFamily_value(child, j);
            int k = sk_IPAddressFamily_find(x->rfc3779_addr, fc);
            IPAddressFamily *fp =
                sk_IPAddressFamily_value(x->rfc3779_addr, k);
            if (fp == NULL) {
                if (fc->ipAddressChoice->type ==
                    IPAddressChoice_addressesOrRanges) {
                    validation_err(X509_V_ERR_UNNESTED_RESOURCE);
                    break;
                }
                continue;
            }
            if (fp->ipAddressChoice->type ==
                IPAddressChoice_addressesOrRanges) {
                if (fc->ipAddressChoice->type == IPAddressChoice_inherit
                    || addr_contains(fp->ipAddressChoice->u.addressesOrRanges,
                                     fc->ipAddressChoice->u.addressesOrRanges,
                                     length_from_afi(X509v3_addr_get_afi(fc))))
                    sk_IPAddressFamily_set(child, j, fp);
                else
                    validation_err(X509_V_ERR_UNNESTED_RESOURCE);
            }
        }
    }

     
    OPENSSL_assert(x != NULL);
    if (x->rfc3779_addr != NULL) {
        for (j = 0; j < sk_IPAddressFamily_num(x->rfc3779_addr); j++) {
            IPAddressFamily *fp =
                sk_IPAddressFamily_value(x->rfc3779_addr, j);
            if (fp->ipAddressChoice->type == IPAddressChoice_inherit
                && sk_IPAddressFamily_find(child, fp) >= 0)
                validation_err(X509_V_ERR_UNNESTED_RESOURCE);
        }
    }

 done:
    sk_IPAddressFamily_free(child);
    return ret;
}