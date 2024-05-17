gs_lib_ctx_set_default_device_list(const gs_memory_t *mem, const char* dev_list_str,
                        int list_str_len)
{
    char *result;
    gs_lib_ctx_t *p_ctx = mem->gs_lib_ctx;
    gs_memory_t *ctx_mem = p_ctx->memory;
    int code = 0;
    
    result = (char *)gs_alloc_bytes(ctx_mem, list_str_len + 1,
             "gs_lib_ctx_set_default_device_list");

    if (result) {
      gs_free_object(ctx_mem, p_ctx->default_device_list,
                "gs_lib_ctx_set_default_device_list");

      memcpy(result, dev_list_str, list_str_len);
      result[list_str_len] = '\0';
      p_ctx->default_device_list = result;
    }
    else {
        code = gs_note_error(gs_error_VMerror);
    }
    return code;
}
