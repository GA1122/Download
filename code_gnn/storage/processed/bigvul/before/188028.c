 Bool PVDecodeVopBody(VideoDecControls *decCtrl, int32 buffer_size[])
 {
     PV_STATUS status = PV_FAIL;
  VideoDecData *video = (VideoDecData *) decCtrl->videoDecoderData;
  int target_layer = video->currLayer;
  Vol *currVol = video->vol[target_layer];
  Vop *currVop = video->currVop;
  Vop *prevVop = video->prevVop;
  Vop *tempVopPtr;
  int bytes_consumed = 0;  
 
  int idx;
 
  if (currVop->vopCoded == 0)  
  {
         PV_BitstreamByteAlign(currVol->bitstream);
   
         bytes_consumed = (getPointer(currVol->bitstream) + 7) >> 3;
 
  if (bytes_consumed > currVol->bitstream->data_end_pos)
  {
             bytes_consumed = currVol->bitstream->data_end_pos;
  }
 
  if (bytes_consumed < buffer_size[target_layer])
  {
   
   
   
             buffer_size[target_layer] -= bytes_consumed;
  }
  else
  {
             buffer_size[target_layer] = 0;
  }
 #ifdef PV_MEMORY_POOL
 
  if (target_layer)
  {
  if (video->prevEnhcVop->timeStamp > video->prevVop->timeStamp)
  {
                 video->prevVop = video->prevEnhcVop;
 
              }
          }
  
//         if (!video->prevVop->yChan) {
//             ALOGE("b/35269635");
//             android_errorWriteLog(0x534e4554, "35269635");
//             return PV_FALSE;
//         }
          oscl_memcpy(currVop->yChan, video->prevVop->yChan, (decCtrl->size*3) / 2);
  
          video->prevVop = prevVop;
 
         video->concealFrame = currVop->yChan;  
 
         video->vop_coding_type = currVop->predictionType;  
 
         decCtrl->outputFrame = currVop->yChan;
 
   
  if (target_layer)
  {
             tempVopPtr = video->prevEnhcVop;
             video->prevEnhcVop = video->currVop;
             video->currVop = tempVopPtr;
  }
  else
  {
             tempVopPtr = video->prevVop;
             video->prevVop = video->currVop;
             video->currVop = tempVopPtr;
  }
 #else
  if (target_layer)  
  {
             video->prevEnhcVop->timeStamp = currVop->timeStamp;
  }
  else
  {
             video->prevVop->timeStamp = currVop->timeStamp;
  }
 #endif
         video->vop_coding_type = currVop->predictionType;  
   
  if (currVop->predictionType == I_VOP)
  {
             video->vop_coding_type = P_VOP;
  }
 
 
  return PV_TRUE;
  }
   
   
   
 
   
  if (decCtrl->nLayers > 1)
  {
  if (currVop->predictionType == I_VOP)
  {
   
  }
  else if (currVop->predictionType == P_VOP)
  {
  switch (currVop->refSelectCode)
  {
  case 0 :  
   
  if (video->prevEnhcVop->timeStamp >= video->prevVop->timeStamp)
                         video->prevVop = video->prevEnhcVop;
  break;
 
  case 1 :  
  if (target_layer)
  {
  if (video->prevEnhcVop->timeStamp > video->prevVop->timeStamp)
                             video->prevVop = video->prevEnhcVop;
  }
  break;
 
  case 2 :  
  break;
 
  case 3 :  
  break;
  }
  }
  else  
  {
             mp4dec_log("DecodeVideoFrame(): B-VOP not supported.\n");
  }
  }
 
   
     idx = ++video->frame_idx % BITRATE_AVERAGE_WINDOW;
 
   
     status = PV_DecodeVop(video);
     video->nBitsPerVop[idx] = getPointer(currVol->bitstream);
     video->prevTimestamp[idx] = currVop->timeStamp;
 
   
     video->prevVop = prevVop;
 
   
     video->duration = video->prevTimestamp[idx];
     video->duration -= video->prevTimestamp[(++idx)%BITRATE_AVERAGE_WINDOW];
  if (video->duration > 0)
  {  
         video->frameRate = (int)(FRAMERATE_SCALE) / video->duration;
  }
 
   
     bytes_consumed = (getPointer(currVol->bitstream) + 7) >> 3;  
 
  if (bytes_consumed > currVol->bitstream->data_end_pos)
  {
         bytes_consumed = currVol->bitstream->data_end_pos;
  }
 
  if (bytes_consumed < buffer_size[target_layer])
  {
   
   
   
         buffer_size[target_layer] -= bytes_consumed;
  }
  else
  {
         buffer_size[target_layer] = 0;
  }
  switch (status)
  {
  case PV_FAIL :
  return PV_FALSE;  
 
  case PV_END_OF_VOP :
   
   
 
  case PV_SUCCESS :
   
 
 
             video->concealFrame = video->currVop->yChan;  
 
             video->vop_coding_type = video->currVop->predictionType;  
 
             decCtrl->outputFrame = video->currVop->yChan;
 
   
  if (target_layer)
  {
                 tempVopPtr = video->prevEnhcVop;
                 video->prevEnhcVop = video->currVop;
                 video->currVop = tempVopPtr;
  }
  else
  {
                 tempVopPtr = video->prevVop;
                 video->prevVop = video->currVop;
                 video->currVop = tempVopPtr;
  }
  break;
 
  default :
   
  break;
  }
 
  return PV_TRUE;
 }