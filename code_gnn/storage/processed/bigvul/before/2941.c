pdf14_copy_alpha_hl_color(gx_device * dev, const byte * data, int data_x,
           int aa_raster, gx_bitmap_id id, int x, int y, int w, int h,
                      const gx_drawing_color *pdcolor, int depth)
{
    return pdf14_copy_alpha_color(dev, data, data_x, aa_raster, id, x, y, w, h,
                                  0, pdcolor, depth, true);
}
