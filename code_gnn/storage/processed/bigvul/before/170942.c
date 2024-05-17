static vpx_image_t *vp8_get_frame(vpx_codec_alg_priv_t *ctx,
 vpx_codec_iter_t *iter)
{
 vpx_image_t *img = NULL;

  
 if (!(*iter) && ctx->yv12_frame_buffers.pbi[0])
 {
        YV12_BUFFER_CONFIG sd;
 int64_t time_stamp = 0, time_end_stamp = 0;
 vp8_ppflags_t flags = {0};

 if (ctx->base.init_flags & VPX_CODEC_USE_POSTPROC)
 {
            flags.post_proc_flag= ctx->postproc_cfg.post_proc_flag
#if CONFIG_POSTPROC_VISUALIZER

 | ((ctx->dbg_color_ref_frame_flag != 0) ? VP8D_DEBUG_CLR_FRM_REF_BLKS : 0)
 | ((ctx->dbg_color_mb_modes_flag != 0) ? VP8D_DEBUG_CLR_BLK_MODES : 0)
 | ((ctx->dbg_color_b_modes_flag != 0) ? VP8D_DEBUG_CLR_BLK_MODES : 0)
 | ((ctx->dbg_display_mv_flag != 0) ? VP8D_DEBUG_DRAW_MV : 0)
#endif
 ;
            flags.deblocking_level      = ctx->postproc_cfg.deblocking_level;
            flags.noise_level           = ctx->postproc_cfg.noise_level;
#if CONFIG_POSTPROC_VISUALIZER
            flags.display_ref_frame_flag= ctx->dbg_color_ref_frame_flag;
            flags.display_mb_modes_flag = ctx->dbg_color_mb_modes_flag;
            flags.display_b_modes_flag  = ctx->dbg_color_b_modes_flag;
            flags.display_mv_flag       = ctx->dbg_display_mv_flag;
#endif
 }

 if (0 == vp8dx_get_raw_frame(ctx->yv12_frame_buffers.pbi[0], &sd,
 &time_stamp, &time_end_stamp, &flags))
 {
            yuvconfig2image(&ctx->img, &sd, ctx->user_priv);

            img = &ctx->img;
 *iter = img;
 }
 }

 return img;
}
