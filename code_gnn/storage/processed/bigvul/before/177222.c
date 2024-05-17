bool ACodec::describeColorFormat(
 const sp<IOMX> &omx, IOMX::node_id node,
 DescribeColorFormatParams &describeParams)
{
    OMX_INDEXTYPE describeColorFormatIndex;
 if (omx->getExtensionIndex(
            node, "OMX.google.android.index.describeColorFormat",
 &describeColorFormatIndex) != OK ||
        omx->getParameter(
            node, describeColorFormatIndex,
 &describeParams, sizeof(describeParams)) != OK) {
 return describeDefaultColorFormat(describeParams);
 }
 return describeParams.sMediaImage.mType !=
 MediaImage::MEDIA_IMAGE_TYPE_UNKNOWN;
}
