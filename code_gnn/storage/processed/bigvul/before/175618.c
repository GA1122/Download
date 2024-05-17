int32 PVGetDecMemoryUsage(VideoDecControls *decCtrl)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
 return video->memoryUsage;
}
