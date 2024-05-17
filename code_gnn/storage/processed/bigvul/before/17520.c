XvResetProcVector(void)
{
#ifdef PANORAMIX
    XvProcVector[xv_PutVideo] = ProcXvPutVideo;
    XvProcVector[xv_PutStill] = ProcXvPutStill;
    XvProcVector[xv_StopVideo] = ProcXvStopVideo;
    XvProcVector[xv_SetPortAttribute] = ProcXvSetPortAttribute;
    XvProcVector[xv_PutImage] = ProcXvPutImage;
    XvProcVector[xv_ShmPutImage] = ProcXvShmPutImage;
#endif
}
