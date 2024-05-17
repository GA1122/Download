AP_DECLARE(void) ap_add_version_component(apr_pool_t *pconf, const char *component)
{
    if (! banner_locked) {
         
        if (server_banner == NULL) {
            apr_pool_cleanup_register(pconf, NULL, reset_banner,
                                      apr_pool_cleanup_null);
            server_banner = apr_pstrdup(pconf, component);
        }
        else {
             
            server_banner = apr_pstrcat(pconf, server_banner, " ",
                                        component, NULL);
        }
    }
    server_description = apr_pstrcat(pconf, server_description, " ",
                                     component, NULL);
}
