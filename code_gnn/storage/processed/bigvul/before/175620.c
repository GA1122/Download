int PVGetLayerID(VideoDecControls *decCtrl)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
 return video->currLayer;
}
