static void ape_unpack_mono(APEContext *ctx, int count)
{
    if (ctx->frameflags & APE_FRAMECODE_STEREO_SILENCE) {
         
        av_log(ctx->avctx, AV_LOG_DEBUG, "pure silence mono\n");
        return;
    }

    ctx->entropy_decode_mono(ctx, count);

     
    ctx->predictor_decode_mono(ctx, count);

     
    if (ctx->channels == 2) {
        memcpy(ctx->decoded[1], ctx->decoded[0], count * sizeof(*ctx->decoded[1]));
    }
}
