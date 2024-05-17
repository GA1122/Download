Bool PVDecSetReference(VideoDecControls *decCtrl, uint8 *refYUV, uint32 timestamp)
{
 VideoDecData *video = (VideoDecData *) decCtrl->videoDecoderData;
 Vop *prevVop = video->prevVop;
 int width = video->width;
 uint8 *dstPtr, *orgPtr, *dstPtr2, *orgPtr2;
 int32 size = (int32)width * video->height;


  
    prevVop->timeStamp = timestamp;
    prevVop->predictionType = I_VOP;

    dstPtr = prevVop->yChan;
    orgPtr = refYUV;
    oscl_memcpy(dstPtr, orgPtr, size);
    dstPtr = prevVop->uChan;
    dstPtr2 = prevVop->vChan;
    orgPtr = refYUV + size;
    orgPtr2 = orgPtr + (size >> 2);
    oscl_memcpy(dstPtr, orgPtr, (size >> 2));
    oscl_memcpy(dstPtr2, orgPtr2, (size >> 2));

    video->concealFrame = video->prevVop->yChan;
    video->vop_coding_type = I_VOP;
    decCtrl->outputFrame = video->prevVop->yChan;

 return PV_TRUE;
}
