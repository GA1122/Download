static vpx_codec_err_t ctrl_get_reference(vpx_codec_alg_priv_t *ctx,
                                          va_list args) {
 vp9_ref_frame_t *data = va_arg(args, vp9_ref_frame_t *);

 if (ctx->frame_parallel_decode) {
    set_error_detail(ctx, "Not supported in frame parallel decode");
 return VPX_CODEC_INCAPABLE;
 }

 if (data) {
    YV12_BUFFER_CONFIG* fb;
 VPxWorker *const worker = ctx->frame_workers;
 FrameWorkerData *const frame_worker_data = (FrameWorkerData *)worker->data1;
    fb = get_ref_frame(&frame_worker_data->pbi->common, data->idx);
 if (fb == NULL) return VPX_CODEC_ERROR;
    yuvconfig2image(&data->img, fb, NULL);
 return VPX_CODEC_OK;
 } else {
 return VPX_CODEC_INVALID_PARAM;
 }
}
