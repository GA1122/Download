static void lua_register_hooks(apr_pool_t *p)
{
     
    ap_hook_handler(lua_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_create_request(create_request_config, NULL, NULL,
                           APR_HOOK_MIDDLE);

     
    ap_hook_translate_name(lua_translate_name_harness_first, NULL, NULL,
                           AP_LUA_HOOK_FIRST);
    ap_hook_translate_name(lua_translate_name_harness, NULL, NULL,
                           APR_HOOK_MIDDLE);
    ap_hook_translate_name(lua_translate_name_harness_last, NULL, NULL,
                           AP_LUA_HOOK_LAST);

    ap_hook_fixups(lua_fixup_harness, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_map_to_storage(lua_map_to_storage_harness, NULL, NULL,
                           APR_HOOK_MIDDLE);

 
    ap_hook_check_user_id(lua_check_user_id_harness, NULL, NULL,
                           APR_HOOK_MIDDLE);
 
    ap_hook_type_checker(lua_type_checker_harness, NULL, NULL,
                         APR_HOOK_MIDDLE);

    ap_hook_access_checker(lua_access_checker_harness_first, NULL, NULL,
                           AP_LUA_HOOK_FIRST);
    ap_hook_access_checker(lua_access_checker_harness, NULL, NULL,
                           APR_HOOK_MIDDLE);
    ap_hook_access_checker(lua_access_checker_harness_last, NULL, NULL,
                           AP_LUA_HOOK_LAST);
    ap_hook_auth_checker(lua_auth_checker_harness_first, NULL, NULL,
                         AP_LUA_HOOK_FIRST);
    ap_hook_auth_checker(lua_auth_checker_harness, NULL, NULL,
                         APR_HOOK_MIDDLE);
    ap_hook_auth_checker(lua_auth_checker_harness_last, NULL, NULL,
                         AP_LUA_HOOK_LAST);

    ap_hook_insert_filter(lua_insert_filter_harness, NULL, NULL,
                          APR_HOOK_MIDDLE);
    ap_hook_quick_handler(lua_quick_harness, NULL, NULL, APR_HOOK_FIRST);

    ap_hook_post_config(lua_post_config, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_pre_config(lua_pre_config, NULL, NULL, APR_HOOK_MIDDLE);

    APR_OPTIONAL_HOOK(ap_lua, lua_open, lua_open_hook, NULL, NULL,
                      APR_HOOK_REALLY_FIRST);

    APR_OPTIONAL_HOOK(ap_lua, lua_request, lua_request_hook, NULL, NULL,
                      APR_HOOK_REALLY_FIRST);
    ap_hook_handler(lua_map_handler, NULL, NULL, AP_LUA_HOOK_FIRST);
    
     
    ap_hook_fixups(lua_map_handler_fixups, NULL, NULL, AP_LUA_HOOK_LAST-2);
#if APR_HAS_THREADS
    ap_hook_child_init(ap_lua_init_mutex, NULL, NULL, APR_HOOK_MIDDLE);
#endif
     
    lua_authz_providers = apr_hash_make(p);
    
     
    ap_hook_log_transaction(lua_log_transaction_harness,NULL,NULL,
                            APR_HOOK_FIRST);
}