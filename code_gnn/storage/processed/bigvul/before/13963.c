gsicc_get_profile_handle_clist(cmm_profile_t *picc_profile, gs_memory_t *memory)
{
    gcmmhprofile_t profile_handle = NULL;
    unsigned int profile_size;
    int size;
    gx_device_clist_reader *pcrdev = (gx_device_clist_reader*) picc_profile->dev;
    unsigned char *buffer_ptr;
    int64_t position;
    gsicc_serialized_profile_t profile_header;
    int k;

    if( pcrdev != NULL) {

         
        position = gsicc_search_icc_table(pcrdev->icc_table,
                                          picc_profile->hashcode, &size);
        if ( position < 0 )
            return 0;   

         
         
        profile_size = size - GSICC_SERIALIZED_SIZE;
         
        buffer_ptr = gs_alloc_bytes(memory->non_gc_memory, profile_size,
                                            "gsicc_get_profile_handle_clist");
        if (buffer_ptr == NULL)
            return 0;
        picc_profile->buffer = buffer_ptr;
        clist_read_chunk(pcrdev, position + GSICC_SERIALIZED_SIZE,
            profile_size, (unsigned char *) buffer_ptr);
        profile_handle = gscms_get_profile_handle_mem(memory->non_gc_memory, buffer_ptr, profile_size);
         
        clist_read_chunk(pcrdev, position, GSICC_SERIALIZED_SIZE,
                        (unsigned char *) (&profile_header));
        picc_profile->buffer_size = profile_header.buffer_size;
        picc_profile->data_cs = profile_header.data_cs;
        picc_profile->default_match = profile_header.default_match;
        picc_profile->hash_is_valid = profile_header.hash_is_valid;
        picc_profile->hashcode = profile_header.hashcode;
        picc_profile->islab = profile_header.islab;
        picc_profile->num_comps = profile_header.num_comps;
        picc_profile->rend_is_valid = profile_header.rend_is_valid;
        picc_profile->rend_cond = profile_header.rend_cond;
        picc_profile->isdevlink = profile_header.isdevlink;
        for ( k = 0; k < profile_header.num_comps; k++ ) {
            picc_profile->Range.ranges[k].rmax =
                profile_header.Range.ranges[k].rmax;
            picc_profile->Range.ranges[k].rmin =
                profile_header.Range.ranges[k].rmin;
        }
        return profile_handle;
     }
     return 0;
}
