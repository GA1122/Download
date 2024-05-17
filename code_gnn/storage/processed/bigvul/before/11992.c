DEFINE_RUN_ONCE_STATIC(ossl_init_load_crypto_nodelete)
{
#ifdef OPENSSL_INIT_DEBUG
    fprintf(stderr, "OPENSSL_INIT: ossl_init_load_crypto_nodelete()\n");
#endif
#if !defined(OPENSSL_USE_NODELETE) \
    && !defined(OPENSSL_NO_PINSHARED)
# if defined(DSO_WIN32) && !defined(_WIN32_WCE)
    {
        HMODULE handle = NULL;
        BOOL ret;

         
        ret = GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
                                | GET_MODULE_HANDLE_EX_FLAG_PIN,
                                (void *)&base_inited, &handle);

#  ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: obtained DSO reference? %s\n",
                (ret == TRUE ? "No!" : "Yes."));
#  endif
        return (ret == TRUE) ? 1 : 0;
    }
# elif !defined(DSO_NONE)
     
    {
        DSO *dso;
        void *err;

        if (!err_shelve_state(&err))
            return 0;

        dso = DSO_dsobyaddr(&base_inited, DSO_FLAG_NO_UNLOAD_ON_FREE);
#  ifdef OPENSSL_INIT_DEBUG
        fprintf(stderr, "OPENSSL_INIT: obtained DSO reference? %s\n",
                (dso == NULL ? "No!" : "Yes."));
         
#  endif
        DSO_free(dso);
        err_unshelve_state(err);
    }
# endif
#endif

    return 1;
}
