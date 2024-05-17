static vpx_codec_err_t vp8_destroy(vpx_codec_alg_priv_t *ctx)
{
    vp8_remove_decoder_instances(&ctx->yv12_frame_buffers);

    vpx_free(ctx);

 return VPX_CODEC_OK;
}
