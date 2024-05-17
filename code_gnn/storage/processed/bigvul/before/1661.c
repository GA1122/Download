gs_pattern1_make_pattern(gs_client_color * pcc,
                         const gs_pattern_template_t * ptemp,
                         const gs_matrix * pmat, gs_gstate * pgs,
                         gs_memory_t * mem)
{
    const gs_pattern1_template_t *pcp = (const gs_pattern1_template_t *)ptemp;
    gs_pattern1_instance_t inst;
    gs_pattern1_instance_t *pinst;
    gs_gstate *saved;
    gs_rect bbox;
    gs_fixed_rect cbox;
    gx_device * pdev = pgs->device;
    int dev_width = pdev->width;
    int dev_height = pdev->height;
    int code = gs_make_pattern_common(pcc, (const gs_pattern_template_t *)pcp,
                                      pmat, pgs, mem,
                                      &st_pattern1_instance);

    if (code < 0)
        return code;
    if (mem == 0)
        mem = gs_gstate_memory(pgs);
    pinst = (gs_pattern1_instance_t *)pcc->pattern;
#ifdef PACIFY_VALGRIND
     
    memset(((char *)&inst) + sizeof(gs_pattern_instance_t), 0,
           sizeof(inst) - sizeof(gs_pattern_instance_t));
#endif
    *(gs_pattern_instance_t *)&inst = *(gs_pattern_instance_t *)pinst;
    saved = inst.saved;
    switch (pcp->PaintType) {
        case 1:          
            gs_set_logical_op(saved, lop_default);
            break;
        case 2:          
            code = gx_set_device_color_1(saved);
            if (code < 0)
                goto fsaved;
            break;
        default:
            code = gs_note_error(gs_error_rangecheck);
            goto fsaved;
    }
    inst.templat = *pcp;
    code = compute_inst_matrix(&inst, saved, &bbox, dev_width, dev_height);
    if (code < 0)
        goto fsaved;

     

    { float width = inst.templat.BBox.q.x - inst.templat.BBox.p.x;
      float height = inst.templat.BBox.q.y - inst.templat.BBox.p.y;

      if ( inst.templat.XStep < width || inst.templat.YStep < height || ctm_only(saved).xy != 0 ||
          ctm_only(saved).yx != 0 ){

          inst.has_overlap = true;

      } else {

          inst.has_overlap = false;

      }

    }

#define mat inst.step_matrix
    if_debug6m('t', mem, "[t]step_matrix=[%g %g %g %g %g %g]\n",
               inst.step_matrix.xx, inst.step_matrix.xy, inst.step_matrix.yx,
               inst.step_matrix.yy, inst.step_matrix.tx, inst.step_matrix.ty);
    if_debug5m('t', mem, "[t]bbox=(%g,%g),(%g,%g), uses_transparency=%d\n",
               bbox.p.x, bbox.p.y, bbox.q.x, bbox.q.y, inst.templat.uses_transparency);
    {
        float bbw = bbox.q.x - bbox.p.x;
        float bbh = bbox.q.y - bbox.p.y;

         
         
        if (ADJUST_SCALE_BY_GS_TRADITION) {
            inst.size.x = (int)(bbw + 0.8);              
            inst.size.y = (int)(bbh + 0.8);
        } else {
            inst.size.x = (int)ceil(bbw);
            inst.size.y = (int)ceil(bbh);
        }

        if (inst.size.x == 0 || inst.size.y == 0) {
             
            gs_make_identity(&inst.step_matrix);
            bbox.p.x = bbox.p.y = bbox.q.x = bbox.q.y = 0;
        } else {
             
            if (fabs(inst.step_matrix.xx * inst.step_matrix.yy - inst.step_matrix.xy * inst.step_matrix.yx) < 1.0e-6) {
                code = gs_note_error(gs_error_rangecheck);
                goto fsaved;
            }
            if (ADJUST_SCALE_BY_GS_TRADITION &&
                inst.step_matrix.xy == 0 && inst.step_matrix.yx == 0 &&
                fabs(fabs(inst.step_matrix.xx) - bbw) < 0.5 &&
                fabs(fabs(inst.step_matrix.yy) - bbh) < 0.5
                ) {
                gs_scale(saved, fabs(inst.size.x / inst.step_matrix.xx),
                         fabs(inst.size.y / inst.step_matrix.yy));
                code = compute_inst_matrix(&inst, saved, &bbox,
                                                dev_width, dev_height);
                if (code < 0)
                    goto fsaved;
                if (ADJUST_SCALE_FOR_THIN_LINES) {
                     
                    gs_scale(saved, (fabs(inst.size.x) - 1.0 / fixed_scale) / fabs(inst.size.x),
                                    (fabs(inst.size.y) - 1.0 / fixed_scale) / fabs(inst.size.y));
                }
                if_debug2m('t', mem,
                           "[t]adjusted XStep & YStep to size=(%d,%d)\n",
                           inst.size.x, inst.size.y);
                if_debug4m('t', mem, "[t]bbox=(%g,%g),(%g,%g)\n",
                           bbox.p.x, bbox.p.y, bbox.q.x, bbox.q.y);
            } else if ((ADJUST_AS_ADOBE) && (inst.templat.TilingType != 2)) {
                if (inst.step_matrix.xy == 0 && inst.step_matrix.yx == 0 &&
                    fabs(fabs(inst.step_matrix.xx) - bbw) < 0.5 &&
                    fabs(fabs(inst.step_matrix.yy) - bbh) < 0.5
                    ) {
                    if (inst.step_matrix.xx <= 2) {
                         
                        gs_scale(saved, fabs(inst.size.x / inst.step_matrix.xx), 1);
                        inst.step_matrix.xx = (float)inst.size.x;
                    } else {
#if 0
                         
                         
                        float newscale = (float)floor(inst.step_matrix.xx + 0.5);
                        gs_scale(saved,
                                 (newscale - 1.0 / fixed_scale) / inst.step_matrix.xx,
                                 1);
                        inst.step_matrix.xx = newscale;
#else
                        inst.step_matrix.xx = (float)floor(inst.step_matrix.xx + 0.5);
                         
                        if (bbw >= inst.size.x - 1.0 / fixed_scale)
                            gs_scale(saved, (fabs(inst.size.x) - 1.0 / fixed_scale) / fabs(inst.size.x), 1);
#endif
                    }
                    if (inst.step_matrix.yy <= 2) {
                        gs_scale(saved, 1, fabs(inst.size.y / inst.step_matrix.yy));
                        inst.step_matrix.yy = (float)inst.size.y;
                    } else {
#if 0
                         
                        float newscale = (float)floor(inst.step_matrix.yy + 0.5);
                        gs_scale(saved,
                                 1,
                                 (newscale - 1.0 / fixed_scale) / inst.step_matrix.yy);
                        inst.step_matrix.yy = newscale;
#else
                        inst.step_matrix.yy = (float)floor(inst.step_matrix.yy + 0.5);
                        if (bbh >= inst.size.y - 1.0 / fixed_scale)
                            gs_scale(saved, 1, (fabs(inst.size.y) - 1.0 / fixed_scale) / fabs(inst.size.y));
#endif
                    }
                    code = gs_bbox_transform(&inst.templat.BBox, &ctm_only(saved), &bbox);
                    if (code < 0)
                        goto fsaved;
                }
            } else if ((inst.templat.TilingType == 2) &&
                       ((pgs->fill_adjust.x | pgs->fill_adjust.y) == 0)) {
                 
                float shiftx = ((inst.step_matrix.yx == 0 &&
                                 fabs(fabs(inst.step_matrix.xx) - bbw) <= 0.5) ?
                                (bbw - inst.size.x)/2 : 0);
                float shifty = ((inst.step_matrix.xy == 0 &&
                                 fabs(fabs(inst.step_matrix.yy) - bbh) <= 0.5) ?
                                (bbh - inst.size.y)/2 : 0);
                gs_translate_untransformed(saved, shiftx, shifty);
                code = gs_bbox_transform(&inst.templat.BBox, &ctm_only(saved), &bbox);
                if (code < 0)
                    goto fsaved;
            }
        }
    }
    if ((code = gs_bbox_transform_inverse(&bbox, &inst.step_matrix, &inst.bbox)) < 0)
        goto fsaved;
    if_debug4m('t', mem, "[t]ibbox=(%g,%g),(%g,%g)\n",
               inst.bbox.p.x, inst.bbox.p.y, inst.bbox.q.x, inst.bbox.q.y);
    inst.is_simple = (fabs(inst.step_matrix.xx) == inst.size.x && inst.step_matrix.xy == 0 &&
                      inst.step_matrix.yx == 0 && fabs(inst.step_matrix.yy) == inst.size.y);
    if_debug6m('t', mem,
               "[t]is_simple? xstep=(%g,%g) ystep=(%g,%g) size=(%d,%d)\n",
               inst.step_matrix.xx, inst.step_matrix.xy,
               inst.step_matrix.yx, inst.step_matrix.yy,
               inst.size.x, inst.size.y);
     
    inst.uses_mask = true;
    inst.is_clist = false;       
    gx_translate_to_fixed(saved, float2fixed_rounded(inst.step_matrix.tx - bbox.p.x),
                                 float2fixed_rounded(inst.step_matrix.ty - bbox.p.y));
    inst.step_matrix.tx = bbox.p.x;
    inst.step_matrix.ty = bbox.p.y;
#undef mat
    cbox.p.x = fixed_0;
    cbox.p.y = fixed_0;
    cbox.q.x = int2fixed(inst.size.x);
    cbox.q.y = int2fixed(inst.size.y);
    code = gx_clip_to_rectangle(saved, &cbox);
    if (code < 0)
        goto fsaved;
    if (!inst.is_simple) {
        code = gs_newpath(saved);
        if (code >= 0)
            code = gs_moveto(saved, inst.templat.BBox.p.x, inst.templat.BBox.p.y);
        if (code >= 0)
            code = gs_lineto(saved, inst.templat.BBox.q.x, inst.templat.BBox.p.y);
        if (code >= 0)
            code = gs_lineto(saved, inst.templat.BBox.q.x, inst.templat.BBox.q.y);
        if (code >= 0)
            code = gs_lineto(saved, inst.templat.BBox.p.x, inst.templat.BBox.q.y);
        if (code >= 0)
            code = gs_clip(saved);
        if (code < 0)
            goto fsaved;
    }
    code = gs_newpath(saved);
    if (code < 0)
        goto fsaved;
    inst.id = gs_next_ids(mem, 1);
    *pinst = inst;
    return 0;
  fsaved:gs_gstate_free(saved);
    gs_free_object(mem, pinst, "gs_makepattern");
    return code;
}
