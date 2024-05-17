gsicc_clone_profile(cmm_profile_t *source, cmm_profile_t **destination,
                    gs_memory_t *memory)
{
    cmm_profile_t *des = gsicc_profile_new(NULL, memory, source->name,
        source->name_length);

    if (des == NULL)
        return gs_throw(gs_error_VMerror, "Profile clone failed");
    des->buffer = gs_alloc_bytes(memory, source->buffer_size, "gsicc_clone_profile");
    if (des->buffer == NULL) {
        rc_decrement(des, "gsicc_clone_profile");
        return gs_throw(gs_error_VMerror, "Profile clone failed");
    }
    memcpy(des->buffer, source->buffer, source->buffer_size);
    des->buffer_size = source->buffer_size;
    gsicc_init_profile_info(des);
    *destination = des;
    return 0;
}
