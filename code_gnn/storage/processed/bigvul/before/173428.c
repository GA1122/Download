OMX_ERRORTYPE omx_vdec::describeColorFormat(OMX_PTR pParam) {

#ifndef FLEXYUV_SUPPORTED
 (void) pParam;
 return OMX_ErrorUndefined;
#else

 if (pParam == NULL) {
        DEBUG_PRINT_ERROR("describeColorFormat: invalid params");
 return OMX_ErrorBadParameter;
 }

 DescribeColorFormatParams *params = (DescribeColorFormatParams*)pParam;

 MediaImage *img = &(params->sMediaImage);
 switch(params->eColorFormat) {
 case QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m:
 {
            img->mType = MediaImage::MEDIA_IMAGE_TYPE_YUV;
            img->mNumPlanes = 3;
            img->mWidth = params->nFrameWidth;
            img->mHeight = params->nFrameHeight;
 size_t planeWidth = VENUS_Y_STRIDE(COLOR_FMT_NV12, params->nFrameWidth);
 size_t planeHeight = VENUS_Y_SCANLINES(COLOR_FMT_NV12, params->nFrameHeight);
            img->mBitDepth = 8;
            img->mPlane[MediaImage::Y].mOffset = 0;
            img->mPlane[MediaImage::Y].mColInc = 1;
            img->mPlane[MediaImage::Y].mRowInc = planeWidth;  
            img->mPlane[MediaImage::Y].mHorizSubsampling = 1;
            img->mPlane[MediaImage::Y].mVertSubsampling = 1;
            img->mPlane[MediaImage::U].mOffset = planeWidth * planeHeight;
            img->mPlane[MediaImage::U].mColInc = 2;  
            img->mPlane[MediaImage::U].mRowInc =
                    VENUS_UV_STRIDE(COLOR_FMT_NV12, params->nFrameWidth);
            img->mPlane[MediaImage::U].mHorizSubsampling = 2;
            img->mPlane[MediaImage::U].mVertSubsampling = 2;
            img->mPlane[MediaImage::V].mOffset = planeWidth * planeHeight + 1;
            img->mPlane[MediaImage::V].mColInc = 2;  
            img->mPlane[MediaImage::V].mRowInc =
                    VENUS_UV_STRIDE(COLOR_FMT_NV12, params->nFrameWidth);
            img->mPlane[MediaImage::V].mHorizSubsampling = 2;
            img->mPlane[MediaImage::V].mVertSubsampling = 2;
 break;
 }

 case OMX_COLOR_FormatYUV420Planar:
 case OMX_COLOR_FormatYUV420SemiPlanar:
 return OMX_ErrorUnsupportedSetting;

 default:
            DEBUG_PRINT_LOW("color-format %x is not flexible", params->eColorFormat);
            img->mType = MediaImage::MEDIA_IMAGE_TYPE_UNKNOWN;
 return OMX_ErrorNone;
 };

    DEBUG_PRINT_LOW("NOTE: Describe color format : %x", params->eColorFormat);
    DEBUG_PRINT_LOW("  FrameWidth x FrameHeight : %d x %d", params->nFrameWidth, params->nFrameHeight);
    DEBUG_PRINT_LOW("  YWidth x YHeight : %d x %d", img->mWidth, img->mHeight);
 for (size_t i = 0; i < img->mNumPlanes; ++i) {
        DEBUG_PRINT_LOW("    Plane[%d] : offset=%d / xStep=%d / yStep = %d",
                i, img->mPlane[i].mOffset, img->mPlane[i].mColInc, img->mPlane[i].mRowInc);
 }
 return OMX_ErrorNone;
#endif  
}
