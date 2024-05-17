static void setup_decoding_thread_data(VP8D_COMP *pbi, MACROBLOCKD *xd, MB_ROW_DEC *mbrd, int count)
{
    VP8_COMMON *const pc = & pbi->common;
 int i;

 for (i = 0; i < count; i++)
 {
        MACROBLOCKD *mbd = &mbrd[i].mbd;
        mbd->subpixel_predict        = xd->subpixel_predict;
        mbd->subpixel_predict8x4     = xd->subpixel_predict8x4;
        mbd->subpixel_predict8x8     = xd->subpixel_predict8x8;
        mbd->subpixel_predict16x16   = xd->subpixel_predict16x16;

        mbd->mode_info_context = pc->mi   + pc->mode_info_stride * (i + 1);
        mbd->mode_info_stride  = pc->mode_info_stride;

        mbd->frame_type = pc->frame_type;
        mbd->pre = xd->pre;
        mbd->dst = xd->dst;

        mbd->segmentation_enabled    = xd->segmentation_enabled;
        mbd->mb_segement_abs_delta     = xd->mb_segement_abs_delta;
        memcpy(mbd->segment_feature_data, xd->segment_feature_data, sizeof(xd->segment_feature_data));

  
        memcpy(mbd->ref_lf_deltas, xd->ref_lf_deltas, sizeof(xd->ref_lf_deltas));
  
        memcpy(mbd->mode_lf_deltas, xd->mode_lf_deltas, sizeof(xd->mode_lf_deltas));
  
        mbd->mode_ref_lf_delta_enabled    = xd->mode_ref_lf_delta_enabled;
        mbd->mode_ref_lf_delta_update    = xd->mode_ref_lf_delta_update;

        mbd->current_bc = &pbi->mbc[0];

        memcpy(mbd->dequant_y1_dc, xd->dequant_y1_dc, sizeof(xd->dequant_y1_dc));
        memcpy(mbd->dequant_y1, xd->dequant_y1, sizeof(xd->dequant_y1));
        memcpy(mbd->dequant_y2, xd->dequant_y2, sizeof(xd->dequant_y2));
        memcpy(mbd->dequant_uv, xd->dequant_uv, sizeof(xd->dequant_uv));

        mbd->fullpixel_mask = 0xffffffff;

 if (pc->full_pixel)
            mbd->fullpixel_mask = 0xfffffff8;

 }

 for (i = 0; i < pc->mb_rows; i++)
        pbi->mt_current_mb_col[i] = -1;
}
