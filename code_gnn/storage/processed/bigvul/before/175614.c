OSCL_EXPORT_REF void PVGetBufferDimensions(VideoDecControls *decCtrl, int32 *width, int32 *height) {
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
 *width = video->width;
 *height = video->height;
}
