static vpx_codec_err_t vp8_get_reference(vpx_codec_alg_priv_t *ctx,
                                         va_list args)
{

 vpx_ref_frame_t *data = va_arg(args, vpx_ref_frame_t *);

 if (data && !ctx->yv12_frame_buffers.use_frame_threads)
 {
 vpx_ref_frame_t *frame = (vpx_ref_frame_t *)data;
        YV12_BUFFER_CONFIG sd;

        image2yuvconfig(&frame->img, &sd);

 return vp8dx_get_reference(ctx->yv12_frame_buffers.pbi[0],
                                   frame->frame_type, &sd);
 }
 else
 return VPX_CODEC_INVALID_PARAM;

}
