gsicc_get_profile_handle_file(const char* pname, int namelen, gs_memory_t *mem)
{
    cmm_profile_t *result;
    stream* str;
    int code;

     
    code = gsicc_open_search(pname, namelen, mem, NULL, 0, &str);
    if (code < 0 || str == NULL) {
        gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
        return NULL;
    }
    result = gsicc_profile_new(str, mem, pname, namelen);
    code = sfclose(str);
    if (result == NULL) {
        gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
        return NULL;
    }
    code = gsicc_init_profile_info(result);
    if (code < 0) {
        gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
        return NULL;
    }
    return result;
}
