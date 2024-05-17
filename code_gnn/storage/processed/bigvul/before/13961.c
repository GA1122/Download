gsicc_get_devicen_names(cmm_profile_t *icc_profile, gs_memory_t *memory)
{
     
    if (icc_profile->profile_handle == NULL) {
        if (icc_profile->buffer != NULL) {
            icc_profile->profile_handle =
                gsicc_get_profile_handle_buffer(icc_profile->buffer,
                                                icc_profile->buffer_size,
                                                memory);
        } else
            return;
    }
    icc_profile->spotnames =
        gsicc_get_spotnames(icc_profile->profile_handle, memory->non_gc_memory);
    return;
}
