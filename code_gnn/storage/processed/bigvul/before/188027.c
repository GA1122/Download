 void SkippedMBMotionComp(
  VideoDecData *video
 )
 {
  Vop *prev = video->prevVop;
  Vop *comp;
  int ypos, xpos;
     PIXEL *c_comp, *c_prev;
     PIXEL *cu_comp, *cu_prev;
     PIXEL *cv_comp, *cv_prev;
  int width, width_uv;
  int32 offset;
 #ifdef PV_POSTPROC_ON  
  int imv;
  int32 size = (int32) video->nTotalMB << 8;
  uint8 *pp_dec_y, *pp_dec_u;
  uint8 *pp_prev1;
  int mvwidth = video->nMBPerRow << 1;
 #endif
 
     width = video->width;
     width_uv  = width >> 1;
     ypos = video->mbnum_row << 4 ;
     xpos = video->mbnum_col << 4 ;
     offset = (int32)ypos * width + xpos;
 
 
  
       
       
    c_prev  = prev->yChan + offset;
//     c_prev  = prev->yChan;
//     if (!c_prev) {
//         ALOGE("b/35269635");
//         android_errorWriteLog(0x534e4554, "35269635");
//         return;
//     }
//     c_prev += offset;
// 
       
      cu_prev = prev->uChan + (offset >> 2) + (xpos >> 2);
       
     cv_prev = prev->vChan + (offset >> 2) + (xpos >> 2);
 
     comp = video->currVop;
 
     c_comp  = comp->yChan + offset;
     cu_comp = comp->uChan + (offset >> 2) + (xpos >> 2);
     cv_comp = comp->vChan + (offset >> 2) + (xpos >> 2);
 
 
   
  PutSKIPPED_MB(c_comp,  c_prev, width);
  PutSKIPPED_B(cu_comp, cu_prev, width_uv);
  PutSKIPPED_B(cv_comp, cv_prev, width_uv);
 
   
 #ifdef PV_POSTPROC_ON  
  if (video->postFilterType != PV_NO_POST_PROC)
  {
         imv = (offset >> 6) - (xpos >> 6) + (xpos >> 3);
   
         pp_prev1 = video->pstprcTypPrv + imv;
         pp_dec_y = video->pstprcTypCur + imv;
  *pp_dec_y = *pp_prev1;
  *(pp_dec_y + 1) = *(pp_prev1 + 1);
  *(pp_dec_y + mvwidth) = *(pp_prev1 + mvwidth);
  *(pp_dec_y + mvwidth + 1) = *(pp_prev1 + mvwidth + 1);
 
   
   
         pp_prev1 = video->pstprcTypPrv + (size >> 6) +
  ((imv + (xpos >> 3)) >> 2);
         pp_dec_u = video->pstprcTypCur + (size >> 6) +
  ((imv + (xpos >> 3)) >> 2);
  *pp_dec_u = *pp_prev1;
         pp_dec_u[size>>8] = pp_prev1[size>>8];
  }
 #endif
   
 
  return;
 }