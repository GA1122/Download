int PVGetDecFramerate(VideoDecControls *decCtrl)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;

 return video->frameRate;
}
