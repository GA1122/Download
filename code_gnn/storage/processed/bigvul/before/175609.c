void PVDecPostProcess(VideoDecControls *decCtrl, uint8 *outputYUV)
{
 uint8 *outputBuffer;
#ifdef PV_POSTPROC_ON
 VideoDecData *video = (VideoDecData *) decCtrl->videoDecoderData;
 int32 tmpvar;
 if (outputYUV)
 {
        outputBuffer = outputYUV;
 }
 else
 {
 if (video->postFilterType)
 {
            outputBuffer = video->currVop->yChan;
 }
 else
 {
            outputBuffer = decCtrl->outputFrame;
 }
 }

 if (video->postFilterType)
 {
  
 PostFilter(video, video->postFilterType, outputBuffer);
 }
 else
 {
 if (outputYUV)
 {
  
            tmpvar = (int32)video->width * video->height;
            oscl_memcpy(outputBuffer, decCtrl->outputFrame, tmpvar*3 / 2);  
 }
 }
#else
    outputBuffer = decCtrl->outputFrame;
    outputYUV;
#endif
    decCtrl->outputFrame = outputBuffer;
 return;
}
