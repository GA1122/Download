gsicc_get_profile_handle_buffer(unsigned char *buffer, int profile_size, gs_memory_t *memory)
{

    gcmmhprofile_t profile_handle = NULL;

     if( buffer != NULL) {
         if (profile_size < ICC_HEADER_SIZE) {
             return 0;
         }
         profile_handle = gscms_get_profile_handle_mem(memory->non_gc_memory, buffer, profile_size);
         return profile_handle;
     }
     return 0;
}
