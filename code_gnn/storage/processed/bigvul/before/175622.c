Bool PVGetVolInfo(VideoDecControls *decCtrl, VolInfo *pVolInfo)
{
 Vol *currVol;

 if (pVolInfo == NULL || decCtrl == NULL || decCtrl->videoDecoderData == NULL ||
 ((VideoDecData *)decCtrl->videoDecoderData)->vol[0] == NULL) return PV_FALSE;

    currVol = ((VideoDecData *)(decCtrl->videoDecoderData))->vol[0];

    pVolInfo->shortVideoHeader = (int32)((VideoDecData *)(decCtrl->videoDecoderData))->shortVideoHeader;
    pVolInfo->dataPartitioning = (int32)currVol->dataPartitioning;
    pVolInfo->errorResDisable  = (int32)currVol->errorResDisable;
    pVolInfo->useReverseVLC    = (int32)currVol->useReverseVLC;
    pVolInfo->scalability      = (int32)currVol->scalability;
    pVolInfo->nbitsTimeIncRes  = (int32)currVol->nbitsTimeIncRes;
    pVolInfo->profile_level_id = (int32)currVol->profile_level_id;

 return PV_TRUE;
}
