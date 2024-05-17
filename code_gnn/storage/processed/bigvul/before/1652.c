compute_inst_matrix(gs_pattern1_instance_t * pinst, gs_gstate * saved,
                            gs_rect * pbbox, int width, int height)
{
    float xx, xy, yx, yy, dx, dy, temp;
    int code;

    code = gs_bbox_transform(&pinst->templat.BBox, &ctm_only(saved), pbbox);
    if (code < 0)
        return code;
     
    dx = pbbox->p.x - floor(pbbox->p.x + 0.5);
    dy = pbbox->p.y - floor(pbbox->p.y + 0.5);
    pbbox->p.x -= dx;
    pbbox->p.y -= dy;
    pbbox->q.x -= dx;
    pbbox->q.y -= dy;
    if (saved->ctm.txy_fixed_valid) {
        code = gx_translate_to_fixed(saved, float2fixed_rounded(saved->ctm.tx - dx),
                                            float2fixed_rounded(saved->ctm.ty - dy));
    } else {          
        saved->ctm.tx -= dx;
        saved->ctm.ty -= dy;
         
        code = gx_path_translate(saved->path, float2fixed(-dx), float2fixed(-dy));
    }
    if (code < 0)
        return code;

     
    xx = pinst->templat.XStep * saved->ctm.xx;
    xy = pinst->templat.XStep * saved->ctm.xy;
    yx = pinst->templat.YStep * saved->ctm.yx;
    yy = pinst->templat.YStep * saved->ctm.yy;

     
    if (xx == 0 || yy == 0) {  
        temp = xx, xx = yx, yx = temp;
        temp = xy, xy = yy, yy = temp;
    }
    if (xx < 0)
        xx = -xx, xy = -xy;
    if (yy < 0)
        yx = -yx, yy = -yy;
     
    pinst->step_matrix.xx = xx;
    pinst->step_matrix.xy = xy;
    pinst->step_matrix.yx = yx;
    pinst->step_matrix.yy = yy;
    pinst->step_matrix.tx = saved->ctm.tx;
    pinst->step_matrix.ty = saved->ctm.ty;
     
    if ((pbbox->q.x - pbbox->p.x > width || pbbox->q.y - pbbox->p.y > height))
        code = clamp_pattern_bbox(pinst, pbbox, width,
                                        height, &ctm_only(saved));

    return code;
}
