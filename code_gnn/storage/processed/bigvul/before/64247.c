static void init_config_defines(apr_pool_t *pconf)
{
    saved_server_config_defines = ap_server_config_defines;
     
    ap_server_config_defines = apr_array_copy(pconf, ap_server_config_defines);
}
