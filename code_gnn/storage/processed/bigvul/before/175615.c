int PVGetDecBitrate(VideoDecControls *decCtrl)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
 int     idx;
 int32   sum = 0;

 for (idx = 0; idx < BITRATE_AVERAGE_WINDOW; idx++)
 {
        sum += video->nBitsPerVop[idx];
 }
    sum = (sum * video->frameRate) / (10 * BITRATE_AVERAGE_WINDOW);
 return (int) sum;
}
