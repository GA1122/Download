bool ACodec::isFlexibleColorFormat(
 const sp<IOMX> &omx, IOMX::node_id node,
 uint32_t colorFormat, bool usingNativeBuffers, OMX_U32 *flexibleEquivalent) {
 DescribeColorFormatParams describeParams;
 InitOMXParams(&describeParams);
    describeParams.eColorFormat = (OMX_COLOR_FORMATTYPE)colorFormat;
    describeParams.nFrameWidth = 128;
    describeParams.nFrameHeight = 128;
    describeParams.nStride = 128;
    describeParams.nSliceHeight = 128;
    describeParams.bUsingNativeBuffers = (OMX_BOOL)usingNativeBuffers;

    CHECK(flexibleEquivalent != NULL);

 if (!describeColorFormat(omx, node, describeParams)) {
 return false;
 }

 const MediaImage &img = describeParams.sMediaImage;
 if (img.mType == MediaImage::MEDIA_IMAGE_TYPE_YUV) {
 if (img.mNumPlanes != 3 ||
            img.mPlane[img.Y].mHorizSubsampling != 1 ||
            img.mPlane[img.Y].mVertSubsampling != 1) {
 return false;
 }

 if (img.mPlane[img.U].mHorizSubsampling == 2
 && img.mPlane[img.U].mVertSubsampling == 2
 && img.mPlane[img.V].mHorizSubsampling == 2
 && img.mPlane[img.V].mVertSubsampling == 2) {
 if (img.mBitDepth <= 8) {
 *flexibleEquivalent = OMX_COLOR_FormatYUV420Flexible;
 return true;
 }
 }
 }
 return false;
}
