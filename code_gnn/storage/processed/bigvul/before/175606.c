Bool IsIntraFrame(VideoDecControls *decCtrl)
{
 VideoDecData *video = (VideoDecData *)decCtrl->videoDecoderData;
 return (video->vop_coding_type == I_VOP);
}
