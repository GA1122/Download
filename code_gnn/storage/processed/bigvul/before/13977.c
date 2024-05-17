gsicc_load_profile_buffer(cmm_profile_t *profile, stream *s,
                          gs_memory_t *memory)
{
    int                     num_bytes,profile_size;
    unsigned char           *buffer_ptr;
    int                     code;

    code = srewind(s);   
    if (code < 0)
        return code;
     
    code = sfseek(s,0,SEEK_END);
    if (code < 0)
        return code;
    profile_size = sftell(s);
    code = srewind(s);
    if (code < 0)
        return code;
    if (profile_size < ICC_HEADER_SIZE)
        return_error(gs_error_VMerror);
     
   buffer_ptr = gs_alloc_bytes(memory, profile_size,
                                        "gsicc_load_profile");
   if (buffer_ptr == NULL)
        return gs_throw(gs_error_VMerror, "Insufficient memory for profile buffer");
   num_bytes = sfread(buffer_ptr,sizeof(unsigned char),profile_size,s);
   if( num_bytes != profile_size) {
       gs_free_object(memory, buffer_ptr, "gsicc_load_profile");
       return -1;
   }
   profile->buffer = buffer_ptr;
   profile->buffer_size = num_bytes;
   return 0;
}
