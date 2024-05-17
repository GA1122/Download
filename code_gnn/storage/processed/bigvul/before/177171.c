void SoftVideoDecoderOMXComponent::copyYV12FrameToOutputBuffer(
 uint8_t *dst, const uint8_t *srcY, const uint8_t *srcU, const uint8_t *srcV,
 size_t srcYStride, size_t srcUStride, size_t srcVStride) {
 size_t dstYStride = outputBufferWidth();
 size_t dstUVStride = dstYStride / 2;
 size_t dstHeight = outputBufferHeight();
 uint8_t *dstStart = dst;

 for (size_t i = 0; i < mHeight; ++i) {
         memcpy(dst, srcY, mWidth);
         srcY += srcYStride;
         dst += dstYStride;
 }

    dst = dstStart + dstYStride * dstHeight;
 for (size_t i = 0; i < mHeight / 2; ++i) {
         memcpy(dst, srcU, mWidth / 2);
         srcU += srcUStride;
         dst += dstUVStride;
 }

    dst = dstStart + (5 * dstYStride * dstHeight) / 4;
 for (size_t i = 0; i < mHeight / 2; ++i) {
         memcpy(dst, srcV, mWidth / 2);
         srcV += srcVStride;
         dst += dstUVStride;
 }
}
