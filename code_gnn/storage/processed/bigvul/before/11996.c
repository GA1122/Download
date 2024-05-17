DEFINE_RUN_ONCE_STATIC(ossl_init_config)
{
    int ret = openssl_config_int(conf_settings);
    config_inited = 1;
    return ret;
}
