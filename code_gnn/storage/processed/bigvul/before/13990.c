gsicc_read_serial_icc(gx_device *dev, int64_t icc_hashcode)
{
    cmm_profile_t *profile;
    int64_t position;
    int size;
    int code;
    gx_device_clist_reader *pcrdev = (gx_device_clist_reader*) dev;

     
    profile = gsicc_profile_new(NULL, pcrdev->memory, NULL, 0);
    if (profile == NULL)
        return NULL;

     
    if (pcrdev->icc_table == NULL) {
        code = clist_read_icctable(pcrdev);
        if (code<0)
            return NULL;
    }
    position = gsicc_search_icc_table(pcrdev->icc_table, icc_hashcode, &size);
    if ( position < 0 )
        return NULL;

     
    clist_read_chunk(pcrdev, position, GSICC_SERIALIZED_SIZE,
                    (unsigned char *) profile);
    return profile;
}
