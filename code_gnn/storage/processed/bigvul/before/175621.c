uint32 PVGetVideoTimeStamp(VideoDecControls *decCtrl)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
 return video->currTimestamp;
}
