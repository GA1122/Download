gs_makebitmappattern_xform(
                              gs_client_color * pcc,
                              const gx_tile_bitmap * ptile,
                              bool mask,
                              const gs_matrix * pmat,
                              long id,
                              gs_gstate * pgs,
                              gs_memory_t * mem
)
{
    gs_depth_bitmap bitmap;

     
    bitmap.data = ptile->data;
    bitmap.raster = ptile->raster;
    bitmap.size.x = ptile->rep_width;
    bitmap.size.y = ptile->rep_height;
    bitmap.id = ptile->id;       
    bitmap.pix_depth = 1;
    bitmap.num_comps = 1;

    return gs_makepixmappattern(pcc, &bitmap, mask, pmat, id, 0, 0, pgs, mem);
}
