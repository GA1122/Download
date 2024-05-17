Bool PVDecodeVopHeader(VideoDecControls *decCtrl, uint8 *buffer[],
 uint32 timestamp[], int32 buffer_size[], VopHeaderInfo *header_info, uint use_ext_timestamp [], uint8 *currYUV)
{
 VideoDecData *video = (VideoDecData *) decCtrl->videoDecoderData;
 Vol *currVol;
 Vop *currVop = video->currVop;
 Vop **vopHeader = video->vopHeader;
 BitstreamDecVideo *stream;

 int target_layer;

#ifdef PV_SUPPORT_TEMPORAL_SCALABILITY
    PV_STATUS status = PV_FAIL;
 int idx;
 int32 display_time;

  
 if (decCtrl->nLayers > 1)
 {
        display_time = target_layer = -1;
 for (idx = 0; idx < decCtrl->nLayers; idx++)
 {
  
 if (buffer_size[idx] <= 0)
 {
                timestamp[idx] = -1;
 continue;
 }

  
 if (timestamp[idx] < 0)
 {
 if (vopHeader[idx]->timeStamp < 0)
 {
  
                    video->currLayer = idx;
                    stream = video->vol[idx]->bitstream;
 BitstreamReset(stream, buffer[idx], buffer_size[idx]);

 while ((status = DecodeVOPHeader(video, vopHeader[idx], FALSE)) != PV_SUCCESS)
 {
  
 if (PVSearchNextM4VFrame(stream) != PV_SUCCESS)
 {
  
  
                            buffer_size[idx] = 0;
 break;
 }
 }
 if (status == PV_SUCCESS)
 {
                        vopHeader[idx]->timeStamp =
                            timestamp[idx] = CalcVopDisplayTime(video->vol[idx], vopHeader[idx], video->shortVideoHeader);
 if (idx == 0) vopHeader[idx]->refSelectCode = 1;
 }
 }
 else
 {
  
                    timestamp[idx] = vopHeader[idx]->timeStamp;
 }
 }

  
 if (timestamp[idx] >= 0 && (display_time < 0 || display_time > timestamp[idx]))
 {
                display_time = timestamp[idx];
                target_layer = idx;
 }
 else if (display_time == timestamp[idx])
 {
  
 }
 }
 if (target_layer < 0) return PV_FALSE;

  
        video->currLayer = target_layer;
        currVol = video->vol[target_layer];
        video->bitstream = stream = currVol->bitstream;

  
  
 if (vopHeader[target_layer]->timeStamp < 0)
 {
            stream = video->vol[target_layer]->bitstream;
 BitstreamReset(stream, buffer[target_layer], buffer_size[target_layer]);

 while (DecodeVOPHeader(video, vopHeader[target_layer], TRUE) != PV_SUCCESS)
 {
  
 if (PVSearchNextM4VFrame(stream) != PV_SUCCESS)
 {
  
  
                    buffer_size[target_layer] = 0;
 break;
 }
 }
            video->vol[target_layer]->timeInc_offset = vopHeader[target_layer]->timeInc;
            video->vol[target_layer]->moduloTimeBase = timestamp[target_layer];
            vopHeader[target_layer]->timeStamp = timestamp[target_layer];
 if (target_layer == 0) vopHeader[target_layer]->refSelectCode = 1;
 }
 }
 else  
 {
#endif
        video->currLayer = target_layer = 0;
        currVol = video->vol[0];
        video->bitstream = stream = currVol->bitstream;
 if (buffer_size[0] <= 0) return PV_FALSE;
 BitstreamReset(stream, buffer[0], buffer_size[0]);

 if (video->shortVideoHeader)
 {
 while (DecodeShortHeader(video, vopHeader[0]) != PV_SUCCESS)
 {
 if (PVSearchNextH263Frame(stream) != PV_SUCCESS)
 {
  
  
                    buffer_size[0] = 0;
 if (video->initialized == PV_FALSE)
 {
                        video->displayWidth = video->width = 0;
                        video->displayHeight = video->height = 0;
 }
 return PV_FALSE;
 }
 }

 if (use_ext_timestamp[0])
 {
  
  
                video->vol[0]->moduloTimeBase = 30 * ((timestamp[0] + 17) / 1001) + (30 * ((timestamp[0] + 17) % 1001) / 1001);
                vopHeader[0]->timeStamp = timestamp[0];
 }
 else
                vopHeader[0]->timeStamp = CalcVopDisplayTime(currVol, vopHeader[0], video->shortVideoHeader);
 }
 else
 {
 while (DecodeVOPHeader(video, vopHeader[0], FALSE) != PV_SUCCESS)
 {
  
 if (PVSearchNextM4VFrame(stream) != PV_SUCCESS)
 {
  
  
                    buffer_size[0] = 0;
 return PV_FALSE;
 }
 }

 if (use_ext_timestamp[0])
 {
                video->vol[0]->timeInc_offset = vopHeader[0]->timeInc;
                video->vol[0]->moduloTimeBase = timestamp[0];  
                vopHeader[0]->timeStamp = timestamp[0];
 }
 else
 {
                vopHeader[0]->timeStamp = CalcVopDisplayTime(currVol, vopHeader[0], video->shortVideoHeader);
 }
 }

  
        vopHeader[0]->refSelectCode = 1;
#ifdef PV_SUPPORT_TEMPORAL_SCALABILITY
 }
#endif
    timestamp[target_layer] = video->currTimestamp = vopHeader[target_layer]->timeStamp;
#ifdef PV_MEMORY_POOL
    vopHeader[target_layer]->yChan = (PIXEL *)currYUV;
    vopHeader[target_layer]->uChan = (PIXEL *)currYUV + decCtrl->size;
    vopHeader[target_layer]->vChan = (PIXEL *)(vopHeader[target_layer]->uChan) + (decCtrl->size >> 2);
#else
    vopHeader[target_layer]->yChan = currVop->yChan;
    vopHeader[target_layer]->uChan = currVop->uChan;
    vopHeader[target_layer]->vChan = currVop->vChan;
#endif
    oscl_memcpy(currVop, vopHeader[target_layer], sizeof(Vop));

#ifdef PV_SUPPORT_TEMPORAL_SCALABILITY
    vopHeader[target_layer]->timeStamp = -1;
#endif
  
    header_info->currLayer = target_layer;
    header_info->timestamp = video->currTimestamp;
    header_info->frameType = (MP4FrameType)currVop->predictionType;
    header_info->refSelCode = vopHeader[target_layer]->refSelectCode;
    header_info->quantizer = currVop->quantizer;
  

 return PV_TRUE;
}
