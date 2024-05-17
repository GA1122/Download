gsicc_profile_new(stream *s, gs_memory_t *memory, const char* pname,
                  int namelen)
{
    cmm_profile_t *result;
    int code;
    char *nameptr = NULL;
    gs_memory_t *mem_nongc = memory->non_gc_memory;

    result = (cmm_profile_t*) gs_alloc_bytes(mem_nongc, sizeof(cmm_profile_t),
                                    "gsicc_profile_new");
    if (result == NULL)
        return result;
    memset(result, 0, GSICC_SERIALIZED_SIZE);
    if (namelen > 0) {
        nameptr = (char*) gs_alloc_bytes(mem_nongc, namelen+1,
                             "gsicc_profile_new");
        if (nameptr == NULL) {
            gs_free_object(mem_nongc, result, "gsicc_profile_new");
            return NULL;
        }
        memcpy(nameptr, pname, namelen);
        nameptr[namelen] = '\0';
        result->name = nameptr;
    } else {
        result->name = NULL;
    }
    result->name_length = namelen;

     
    if ( s != NULL) {
        code = gsicc_load_profile_buffer(result, s, mem_nongc);
        if (code < 0) {
            gs_free_object(mem_nongc, result, "gsicc_profile_new");
            gs_free_object(mem_nongc, nameptr, "gsicc_profile_new");
            return NULL;
        }
    } else {
        result->buffer = NULL;
        result->buffer_size = 0;
    }
    rc_init_free(result, mem_nongc, 1, rc_free_icc_profile);
    result->profile_handle = NULL;
    result->spotnames = NULL;
    result->rend_is_valid = false;
    result->isdevlink = false;   
    result->dev = NULL;
    result->memory = mem_nongc;
    result->lock = gx_monitor_label(gx_monitor_alloc(mem_nongc),
                                    "gsicc_manage");
    result->vers = ICCVERS_UNKNOWN;
    result->v2_data = NULL;
    result->v2_size = 0;
    result->release = gscms_release_profile;  

    if (result->lock == NULL) {
        gs_free_object(mem_nongc, result, "gsicc_profile_new");
        gs_free_object(mem_nongc, nameptr, "gsicc_profile_new");
        return NULL;
    }
    if_debug1m(gs_debug_flag_icc, mem_nongc,
               "[icc] allocating ICC profile = 0x%p\n", result);
    return result;
}
