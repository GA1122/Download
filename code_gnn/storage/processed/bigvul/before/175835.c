vpx_codec_err_t vp8dx_set_reference(VP8D_COMP *pbi, enum vpx_ref_frame_type ref_frame_flag, YV12_BUFFER_CONFIG *sd)
{
    VP8_COMMON *cm = &pbi->common;
 int *ref_fb_ptr = NULL;
 int free_fb;

 if (ref_frame_flag == VP8_LAST_FRAME)
        ref_fb_ptr = &cm->lst_fb_idx;
 else if (ref_frame_flag == VP8_GOLD_FRAME)
        ref_fb_ptr = &cm->gld_fb_idx;
 else if (ref_frame_flag == VP8_ALTR_FRAME)
        ref_fb_ptr = &cm->alt_fb_idx;
 else{
        vpx_internal_error(&pbi->common.error, VPX_CODEC_ERROR,
 "Invalid reference frame");
 return pbi->common.error.error_code;
 }

 if(cm->yv12_fb[*ref_fb_ptr].y_height != sd->y_height ||
        cm->yv12_fb[*ref_fb_ptr].y_width != sd->y_width ||
        cm->yv12_fb[*ref_fb_ptr].uv_height != sd->uv_height ||
        cm->yv12_fb[*ref_fb_ptr].uv_width != sd->uv_width){
        vpx_internal_error(&pbi->common.error, VPX_CODEC_ERROR,
 "Incorrect buffer dimensions");
 }
 else{
  
        free_fb = get_free_fb(cm);
  
        cm->fb_idx_ref_cnt[free_fb]--;

  
        ref_cnt_fb (cm->fb_idx_ref_cnt, ref_fb_ptr, free_fb);
        vp8_yv12_copy_frame(sd, &cm->yv12_fb[*ref_fb_ptr]);
 }

 return pbi->common.error.error_code;
}
