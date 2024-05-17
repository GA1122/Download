OPJ_BOOL opj_alloc_tile_component_data(opj_tcd_tilecomp_t *l_tilec)
{
    if ((l_tilec->data == 00) ||
            ((l_tilec->data_size_needed > l_tilec->data_size) &&
             (l_tilec->ownsData == OPJ_FALSE))) {
        l_tilec->data = (OPJ_INT32 *) opj_image_data_alloc(l_tilec->data_size_needed);
        if (! l_tilec->data) {
            return OPJ_FALSE;
        }
         
        l_tilec->data_size = l_tilec->data_size_needed;
        l_tilec->ownsData = OPJ_TRUE;
    } else if (l_tilec->data_size_needed > l_tilec->data_size) {
         
        opj_image_data_free(l_tilec->data);
        l_tilec->data = (OPJ_INT32 *) opj_image_data_alloc(l_tilec->data_size_needed);
        if (! l_tilec->data) {
            l_tilec->data_size = 0;
            l_tilec->data_size_needed = 0;
            l_tilec->ownsData = OPJ_FALSE;
            return OPJ_FALSE;
        }
         
        l_tilec->data_size = l_tilec->data_size_needed;
        l_tilec->ownsData = OPJ_TRUE;
    }
    return OPJ_TRUE;
}