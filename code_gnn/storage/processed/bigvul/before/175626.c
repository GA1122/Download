OSCL_EXPORT_REF void PVSetPostProcType(VideoDecControls *decCtrl, int mode)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
    video->postFilterType = mode;
}
