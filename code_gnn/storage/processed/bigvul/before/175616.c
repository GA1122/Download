OSCL_EXPORT_REF MP4DecodingMode PVGetDecBitstreamMode(VideoDecControls *decCtrl)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
 if (video->shortVideoHeader)
 {
 return H263_MODE;
 }
 else
 {
 return MPEG4_MODE;
 }
}
