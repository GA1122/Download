gs_lib_ctx_get_default_device_list(const gs_memory_t *mem, char** dev_list_str,
                        int *list_str_len)
{
     
    if (mem && mem->gs_lib_ctx && mem->gs_lib_ctx->default_device_list) {
        *dev_list_str = mem->gs_lib_ctx->default_device_list;
    }
    else {
        *dev_list_str = (char *)gs_dev_defaults;
    }

    *list_str_len = strlen(*dev_list_str);

    return 0;
}
