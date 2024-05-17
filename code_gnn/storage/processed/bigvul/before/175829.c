static void remove_decompressor(VP8D_COMP *pbi)
{
#if CONFIG_ERROR_CONCEALMENT
    vp8_de_alloc_overlap_lists(pbi);
#endif
    vp8_remove_common(&pbi->common);
    vpx_free(pbi);
}
