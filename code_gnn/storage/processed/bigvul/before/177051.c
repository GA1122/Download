SoftVideoDecoderOMXComponent::CropSettingsMode SoftAVC::handleCropParams(
 const H264SwDecInfo& decInfo) {
 if (!decInfo.croppingFlag) {
 return kCropUnSet;
 }

 const CropParams& crop = decInfo.cropParams;
 if (mCropLeft == crop.cropLeftOffset &&
        mCropTop == crop.cropTopOffset &&
        mCropWidth == crop.cropOutWidth &&
        mCropHeight == crop.cropOutHeight) {
 return kCropSet;
 }

    mCropLeft = crop.cropLeftOffset;
    mCropTop = crop.cropTopOffset;
    mCropWidth = crop.cropOutWidth;
    mCropHeight = crop.cropOutHeight;
 return kCropChanged;
}
