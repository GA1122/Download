zcolor_remap_color(i_ctx_t *i_ctx_p)
{
     
    gs_swapcolors(igs);
    gx_unset_dev_color(igs);
    gs_swapcolors(igs);
    gx_unset_dev_color(igs);
    return 0;
}