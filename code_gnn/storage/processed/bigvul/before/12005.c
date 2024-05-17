DEFINE_RUN_ONCE_STATIC(ossl_init_zlib)
{
     
    zlib_inited = 1;
    return 1;
}
