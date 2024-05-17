OSCL_EXPORT_REF Bool PVInitVideoDecoder(VideoDecControls *decCtrl, uint8 *volbuf[],
 int32 *volbuf_size, int nLayers, int width, int height, MP4DecodingMode mode)
{
 VideoDecData *video = (VideoDecData *) decCtrl->videoDecoderData;
 Bool status = PV_TRUE;
 int idx;
 BitstreamDecVideo *stream;


    oscl_memset(decCtrl, 0, sizeof(VideoDecControls));  
    decCtrl->nLayers = nLayers;
 for (idx = 0; idx < nLayers; idx++)
 {
        decCtrl->volbuf[idx] = volbuf[idx];
        decCtrl->volbuf_size[idx] = volbuf_size[idx];
 }

  
#ifdef DEC_INTERNAL_MEMORY_OPT
    video = IMEM_VideoDecData;
#else
    video = (VideoDecData *) oscl_malloc(sizeof(VideoDecData));
#endif
 if (video != NULL)
 {
        oscl_memset(video, 0, sizeof(VideoDecData));
        video->memoryUsage = sizeof(VideoDecData);
        video->numberOfLayers = nLayers;
#ifdef DEC_INTERNAL_MEMORY_OPT
        video->vol = (Vol **) IMEM_VOL;
#else
 if ((size_t)nLayers > SIZE_MAX / sizeof(Vol *)) {
            status = PV_FALSE;
 goto fail;
 }

        video->vol = (Vol **) oscl_malloc(nLayers * sizeof(Vol *));
#endif
 if (video->vol == NULL) status = PV_FALSE;
        video->memoryUsage += nLayers * sizeof(Vol *);


  
  
        decCtrl->videoDecoderData = (void *) video;
        video->videoDecControls = decCtrl;  
  

  
  
#ifdef DEC_INTERNAL_MEMORY_OPT
        video->currVop = IMEM_currVop;
 if (video->currVop == NULL) status = PV_FALSE;
 else oscl_memset(video->currVop, 0, sizeof(Vop));
        video->prevVop = IMEM_prevVop;
 if (video->prevVop == NULL) status = PV_FALSE;
 else oscl_memset(video->prevVop, 0, sizeof(Vop));
        video->memoryUsage += (sizeof(Vop) * 2);
        video->vopHeader = (Vop **) IMEM_vopHEADER;
#else

        video->currVop = (Vop *) oscl_malloc(sizeof(Vop));
 if (video->currVop == NULL) status = PV_FALSE;
 else oscl_memset(video->currVop, 0, sizeof(Vop));
        video->prevVop = (Vop *) oscl_malloc(sizeof(Vop));
 if (video->prevVop == NULL) status = PV_FALSE;
 else oscl_memset(video->prevVop, 0, sizeof(Vop));
        video->memoryUsage += (sizeof(Vop) * 2);

 if ((size_t)nLayers > SIZE_MAX / sizeof(Vop *)) {
            status = PV_FALSE;
 goto fail;
 }

        video->vopHeader = (Vop **) oscl_malloc(sizeof(Vop *) * nLayers);
#endif
 if (video->vopHeader == NULL) status = PV_FALSE;
 else oscl_memset(video->vopHeader, 0, sizeof(Vop *)*nLayers);
        video->memoryUsage += (sizeof(Vop *) * nLayers);

        video->initialized = PV_FALSE;
  
 if (status == PV_TRUE)
 {
  
            video->frame_idx = -1;


 for (idx = 0; idx < nLayers; idx++)
 {

#ifdef DEC_INTERNAL_MEMORY_OPT
                video->vopHeader[idx] = IMEM_vopHeader[idx];
#else
                video->vopHeader[idx] = (Vop *) oscl_malloc(sizeof(Vop));
#endif
 if (video->vopHeader[idx] == NULL)
 {
                    status = PV_FALSE;
 break;
 }
 else
 {
                    oscl_memset(video->vopHeader[idx], 0, sizeof(Vop));
                    video->vopHeader[idx]->timeStamp = 0;
                    video->memoryUsage += (sizeof(Vop));
 }
#ifdef DEC_INTERNAL_MEMORY_OPT
                video->vol[idx] = IMEM_vol[idx];
                video->memoryUsage += sizeof(Vol);
                oscl_memset(video->vol[idx], 0, sizeof(Vol));
 if (video->vol[idx] == NULL) status = PV_FALSE;
                stream = IMEM_BitstreamDecVideo;
#else
                video->vol[idx] = (Vol *) oscl_malloc(sizeof(Vol));
 if (video->vol[idx] == NULL)
 {
                    status = PV_FALSE;
 break;
 }
 else
 {
                    video->memoryUsage += sizeof(Vol);
                    oscl_memset(video->vol[idx], 0, sizeof(Vol));
 }

                stream = (BitstreamDecVideo *) oscl_malloc(sizeof(BitstreamDecVideo));
#endif
                video->memoryUsage += sizeof(BitstreamDecVideo);
 if (stream == NULL)
 {
                    status = PV_FALSE;
 break;
 }
 else
 {
 int32 buffer_size;
 if ((buffer_size = BitstreamOpen(stream, idx)) < 0)
 {
                        mp4dec_log("InitVideoDecoder(): Can't allocate bitstream buffer.\n");
                        status = PV_FALSE;
 break;
 }
                    video->memoryUsage += buffer_size;
                    video->vol[idx]->bitstream = stream;
                    video->vol[idx]->volID = idx;
                    video->vol[idx]->timeInc_offset = 0;  
                    video->vlcDecCoeffIntra = &VlcDecTCOEFShortHeader;
                    video->vlcDecCoeffInter = &VlcDecTCOEFShortHeader;
 if (mode == MPEG4_MODE)
 {
  
 BitstreamReset(stream, decCtrl->volbuf[idx], decCtrl->volbuf_size[idx]);

 switch (DecodeVOLHeader(video, idx))
 {
 case PV_SUCCESS :
 if (status == PV_TRUE)
                                    status = PV_TRUE;  
 else
                                    status = PV_FALSE;
 break;
#ifdef PV_TOLERATE_VOL_ERRORS
 case PV_BAD_VOLHEADER:
                                status = PV_TRUE;
 break;
#endif
 default :
                                status = PV_FALSE;
 break;
 }

 }
 else
 {
                        video->shortVideoHeader = PV_TRUE;
 }

 if (video->shortVideoHeader == PV_TRUE)
 {
                        mode = H263_MODE;
  
  
  
                        video->prevVop->temporalRef = 0;  
  
                        video->vol[idx]->quantType = 0;
                        video->vol[idx]->quantPrecision = 5;
                        video->vol[idx]->errorResDisable = 1;
                        video->vol[idx]->dataPartitioning = 0;
                        video->vol[idx]->useReverseVLC = 0;
                        video->intra_acdcPredDisable = 1;
                        video->vol[idx]->scalability = 0;

                        video->displayWidth = width;
                        video->displayHeight = height;
                        video->width = (width + 15) & -16;
                        video->height = (height + 15) & -16;
                        video->size = (int32)video->width * video->height;

#ifdef PV_ANNEX_IJKT_SUPPORT
                        video->modified_quant = 0;
                        video->advanced_INTRA = 0;
                        video->deblocking = 0;
                        video->slice_structure = 0;
#endif
 }

 }
 }

 }
 if (status != PV_FALSE)
 {
            status = PVAllocVideoData(decCtrl, width, height, nLayers);
            video->initialized = PV_TRUE;
 }
 }
 else
 {
        status = PV_FALSE;
 }

fail:
 if (status == PV_FALSE) PVCleanUpVideoDecoder(decCtrl);

 return status;
}
