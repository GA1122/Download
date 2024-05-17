 void MBMotionComp(
  VideoDecData *video,
  int CBP
 )
 {
 
   
   
  Vop *prev = video->prevVop;
 
   
  int mbnum = video->mbnum;
 
   
  int MB_in_width = video->nMBPerRow;
  int ypos, xpos;
     PIXEL *c_comp, *c_prev;
     PIXEL *cu_comp, *cu_prev;
     PIXEL *cv_comp, *cv_prev;
  int height, width, pred_width;
  int imv, mvwidth;
  int32 offset;
  uint8 mode;
  uint8 *pred_block, *pred;
 
   
  int dx, dy;
 
     MOT px[4], py[4];
  int xpred, ypred;
  int xsum;
  int round1;
 #ifdef PV_POSTPROC_ON  
   
  int32 size = (int32) video->nTotalMB << 8;
  uint8 *pp_dec_y, *pp_dec_u;
  int ll[4];
  int tmp = 0;
  uint8 msk_deblock = 0;
 #endif
   
   
   
     round1 = (int)(1 - video->currVop->roundingType);
 
   
     width = video->width;
 
   
     height = video->height;
 
   
     mvwidth = MB_in_width << 1;
 
   
     ypos = video->mbnum_row << 4 ;
   
     xpos = video->mbnum_col << 4 ;
 
   
   
   
     offset = (int32)ypos * width + xpos;
 
   
     mode = video->headerInfo.Mode[mbnum];
 
   
   
     imv = (offset >> 6) - (xpos >> 6) + (xpos >> 3);
  if (mode & INTER_1VMASK)
  {
         dx = px[0] = px[1] = px[2] = px[3] = video->motX[imv];
         dy = py[0] = py[1] = py[2] = py[3] = video->motY[imv];
  if ((dx & 3) == 0)
  {
             dx = dx >> 1;
  }
  else
  {
   
             dx = (dx >> 1) | 1;
  }
 
   
   
  if ((dy & 3) == 0)
  {
             dy = dy >> 1;
  }
  else
  {
   
             dy = (dy >> 1) | 1;
  }
  }
  else
  {
         px[0] = video->motX[imv];
         px[1] = video->motX[imv+1];
         px[2] = video->motX[imv+mvwidth];
         px[3] = video->motX[imv+mvwidth+1];
         xsum = px[0] + px[1] + px[2] + px[3];
         dx = PV_SIGN(xsum) * (roundtab16[(PV_ABS(xsum)) & 0xF] +
  (((PV_ABS(xsum)) >> 4) << 1));
         py[0] = video->motY[imv];
         py[1] = video->motY[imv+1];
         py[2] = video->motY[imv+mvwidth];
         py[3] = video->motY[imv+mvwidth+1];
         xsum = py[0] + py[1] + py[2] + py[3];
         dy = PV_SIGN(xsum) * (roundtab16[(PV_ABS(xsum)) & 0xF] +
  (((PV_ABS(xsum)) >> 4) << 1));
  }
 
  
       
      c_prev  = prev->yChan;
//     if (!c_prev) {
//         ALOGE("b/35269635");
//         android_errorWriteLog(0x534e4554, "35269635");
//         return;
//     }
  
      pred_block = video->mblock->pred_block;
  
   
   
   
   
 
 
 
   
   
     c_comp = video->currVop->yChan + offset;
 
 
     xpred = (int)((xpos << 1) + px[0]);
     ypred = (int)((ypos << 1) + py[0]);
 
  if ((CBP >> 5)&1)
  {
         pred = pred_block;
         pred_width = 16;
  }
  else
  {
         pred = c_comp;
         pred_width = width;
  }
 
   
  if (xpred >= 0 && xpred <= ((width << 1) - (2*B_SIZE)) &&
             ypred >= 0 && ypred <= ((height << 1) - (2*B_SIZE)))
  {  
   
   
  ;
  GetPredAdvBTable[ypred&1][xpred&1](c_prev + (xpred >> 1) + ((ypred >> 1)*width),
                                            pred, width, (pred_width << 1) | round1);
  }
  else
  {  
   
   
  GetPredOutside(xpred, ypred, c_prev,
                        pred, width, height, round1, pred_width);
  }
 
 
   
   
   
     xpred = (int)(((xpos + B_SIZE) << 1) + px[1]);
     ypred = (int)((ypos << 1) + py[1]);
 
  if ((CBP >> 4)&1)
  {
         pred = pred_block + 8;
         pred_width = 16;
  }
  else
  {
         pred = c_comp + 8;
         pred_width = width;
  }
 
   
  if (xpred >= 0 && xpred <= ((width << 1) - (2*B_SIZE)) &&
             ypred >= 0 && ypred <= ((height << 1) - (2*B_SIZE)))
  {  
   
   
  GetPredAdvBTable[ypred&1][xpred&1](c_prev + (xpred >> 1) + ((ypred >> 1)*width),
                                            pred, width, (pred_width << 1) | round1);
  }
  else
  {  
   
   
  GetPredOutside(xpred, ypred, c_prev,
                        pred, width, height, round1, pred_width);
  }
 
 
 
   
   
   
     xpred = (int)((xpos << 1) + px[2]);
     ypred = (int)(((ypos + B_SIZE) << 1) + py[2]);
 
  if ((CBP >> 3)&1)
  {
         pred = pred_block + 128;
         pred_width = 16;
  }
  else
  {
         pred = c_comp + (width << 3);
         pred_width = width;
  }
 
   
  if (xpred >= 0 && xpred <= ((width << 1) - (2*B_SIZE)) &&
             ypred >= 0 && ypred <= ((height << 1) - (2*B_SIZE)))
  {  
   
   
  GetPredAdvBTable[ypred&1][xpred&1](c_prev + (xpred >> 1) + ((ypred >> 1)*width),
                                            pred, width, (pred_width << 1) | round1);
  }
  else
  {  
   
   
  GetPredOutside(xpred, ypred, c_prev,
                        pred, width, height, round1, pred_width);
  }
 
 
 
   
   
   
   
     xpred = (int)(((xpos + B_SIZE) << 1) + px[3]);
     ypred = (int)(((ypos + B_SIZE) << 1) + py[3]);
 
  if ((CBP >> 2)&1)
  {
         pred = pred_block + 136;
         pred_width = 16;
  }
  else
  {
         pred = c_comp + (width << 3) + 8;
         pred_width = width;
  }
 
   
  if (xpred >= 0 && xpred <= ((width << 1) - (2*B_SIZE)) &&
             ypred >= 0 && ypred <= ((height << 1) - (2*B_SIZE)))
  {  
   
   
  GetPredAdvBTable[ypred&1][xpred&1](c_prev + (xpred >> 1) + ((ypred >> 1)*width),
                                            pred, width, (pred_width << 1) | round1);
  }
  else
  {  
   
   
  GetPredOutside(xpred, ypred, c_prev,
                        pred, width, height, round1, pred_width);
  }
   
   
 
 #ifdef PV_POSTPROC_ON
  if (video->postFilterType != PV_NO_POST_PROC)
  {
  if (mode&INTER_1VMASK)
  {
             pp_dec_y = video->pstprcTypCur + imv;
             ll[0] = 1;
             ll[1] = mvwidth - 1;
             ll[2] = 1;
             ll[3] = -mvwidth - 1;
             msk_deblock = pp_semaphore_luma(xpred, ypred, pp_dec_y,
                                             video->pstprcTypPrv, ll, &tmp, px[0], py[0], mvwidth,
                                             width, height);
 
             pp_dec_u = video->pstprcTypCur + (size >> 6) +
  ((imv + (xpos >> 3)) >> 2);
 
             pp_semaphore_chroma_inter(xpred, ypred, pp_dec_u,
                                       video->pstprcTypPrv, dx, dy, mvwidth, height, size,
                                       tmp, msk_deblock);
  }
  else
  {
   
   
             pp_dec_y = video->pstprcTypCur + imv;
  *pp_dec_y = 4;
  *(pp_dec_y + 1) = 4;
  *(pp_dec_y + mvwidth) = 4;
  *(pp_dec_y + mvwidth + 1) = 4;
             pp_dec_u = video->pstprcTypCur + (size >> 6) +
  ((imv + (xpos >> 3)) >> 2);
  *pp_dec_u = 4;
             pp_dec_u[size>>8] = 4;
  }
  }
 #endif
 
 
   
   
 
   
   
     width >>= 1;
 
   
     height >>= 1;
 
   
     cu_prev = prev->uChan;
 
   
     cv_prev = prev->vChan;
 
   
   
   
     xpred = xpos + dx;
 
   
   
   
     ypred = ypos + dy;
 
     cu_comp = video->currVop->uChan + (offset >> 2) + (xpos >> 2);
     cv_comp = video->currVop->vChan + (offset >> 2) + (xpos >> 2);
 
   
   
  if (xpred >= 0 && xpred <= ((width << 1) - (2*B_SIZE)) && ypred >= 0 &&
             ypred <= ((height << 1) - (2*B_SIZE)))
  {
   
   
   
  if ((CBP >> 1)&1)
  {
             pred = pred_block + 256;
             pred_width = 16;
  }
  else
  {
             pred = cu_comp;
             pred_width = width;
  }
 
   
  GetPredAdvBTable[ypred&1][xpred&1](cu_prev + (xpred >> 1) + ((ypred >> 1)*width),
                                            pred, width, (pred_width << 1) | round1);
 
  if (CBP&1)
  {
             pred = pred_block + 264;
             pred_width = 16;
  }
  else
  {
             pred = cv_comp;
             pred_width = width;
  }
   
  GetPredAdvBTable[ypred&1][xpred&1](cv_prev + (xpred >> 1) + ((ypred >> 1)*width),
                                            pred, width, (pred_width << 1) | round1);
 
  return ;
  }
  else
  {
   
   
   
  if ((CBP >> 1)&1)
  {
             pred = pred_block + 256;
             pred_width = 16;
  }
  else
  {
             pred = cu_comp;
             pred_width = width;
  }
 
   
  GetPredOutside(xpred, ypred,    cu_prev,
                        pred, width, height, round1, pred_width);
 
  if (CBP&1)
  {
             pred = pred_block + 264;
             pred_width = 16;
  }
  else
  {
             pred = cv_comp;
             pred_width = width;
  }
 
   
  GetPredOutside(xpred, ypred,    cv_prev,
                        pred, width, height, round1, pred_width);
 
  return ;
  }
 
 }