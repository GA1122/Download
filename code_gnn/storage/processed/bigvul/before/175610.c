Bool PVDecSetEnhReference(VideoDecControls *decCtrl, uint8 *refYUV, uint32 timestamp)
{
 VideoDecData *video = (VideoDecData *) decCtrl->videoDecoderData;
 Vop *prevEnhcVop = video->prevEnhcVop;
 uint8 *dstPtr, *orgPtr, *dstPtr2, *orgPtr2;
 int32 size = (int32) video->width * video->height;

 if (video->numberOfLayers <= 1)
 return PV_FALSE;


  
    prevEnhcVop->timeStamp = timestamp;
    prevEnhcVop->predictionType = I_VOP;

    dstPtr = prevEnhcVop->yChan;
    orgPtr = refYUV;
    oscl_memcpy(dstPtr, orgPtr, size);
    dstPtr = prevEnhcVop->uChan;
    dstPtr2 = prevEnhcVop->vChan;
    orgPtr = refYUV + size;
    orgPtr2 = orgPtr + (size >> 2);
    oscl_memcpy(dstPtr, orgPtr, (size >> 2));
    oscl_memcpy(dstPtr2, orgPtr2, (size >> 2));
    video->concealFrame = video->prevEnhcVop->yChan;
    video->vop_coding_type = I_VOP;
    decCtrl->outputFrame = video->prevEnhcVop->yChan;

 return PV_TRUE;
}
