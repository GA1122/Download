static int check_fragments_for_errors(VP8D_COMP *pbi)
{
 if (!pbi->ec_active &&
        pbi->fragments.count <= 1 && pbi->fragments.sizes[0] == 0)
 {
        VP8_COMMON *cm = &pbi->common;

  
 if (cm->fb_idx_ref_cnt[cm->lst_fb_idx] > 1)
 {
  
 const int prev_idx = cm->lst_fb_idx;
            cm->fb_idx_ref_cnt[prev_idx]--;
            cm->lst_fb_idx = get_free_fb(cm);
            vp8_yv12_copy_frame(&cm->yv12_fb[prev_idx],
 &cm->yv12_fb[cm->lst_fb_idx]);
 }
  
        cm->yv12_fb[cm->lst_fb_idx].corrupted = 1;

  
        cm->show_frame = 0;

  
 return 0;
 }

 return 1;
}
