bool venc_dev::handle_extradata(void *buffer, int index)
{
    OMX_BUFFERHEADERTYPE *p_bufhdr = (OMX_BUFFERHEADERTYPE *) buffer;
    OMX_OTHER_EXTRADATATYPE *p_extra = NULL;

 if (!extradata_info.uaddr) {
        DEBUG_PRINT_ERROR("Extradata buffers not allocated");
 return false;
 }

    p_extra = (OMX_OTHER_EXTRADATATYPE *)ALIGN(p_bufhdr->pBuffer +
                p_bufhdr->nOffset + p_bufhdr->nFilledLen, 4);

 if (extradata_info.buffer_size >
            p_bufhdr->nAllocLen - ALIGN(p_bufhdr->nOffset + p_bufhdr->nFilledLen, 4)) {
        DEBUG_PRINT_ERROR("Insufficient buffer size for extradata");
        p_extra = NULL;
 return false;
 } else if (sizeof(msm_vidc_extradata_header) != sizeof(OMX_OTHER_EXTRADATATYPE)) {
  
        DEBUG_PRINT_ERROR("Extradata ABI mismatch");
 return false;
 }

 struct msm_vidc_extradata_header *p_extradata = NULL;
 do {
        p_extradata = (struct msm_vidc_extradata_header *) (p_extradata ?
 ((char *)p_extradata) + p_extradata->size :
            extradata_info.uaddr + index * extradata_info.buffer_size);

 switch (p_extradata->type) {
 case MSM_VIDC_EXTRADATA_METADATA_MBI:
 {
                OMX_U32 payloadSize = append_mbi_extradata(&p_extra->data, p_extradata);
                p_extra->nSize = ALIGN(sizeof(OMX_OTHER_EXTRADATATYPE) + payloadSize, 4);
                p_extra->nVersion.nVersion = OMX_SPEC_VERSION;
                p_extra->nPortIndex = OMX_DirOutput;
                p_extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataVideoEncoderMBInfo;
                p_extra->nDataSize = payloadSize;
 break;
 }
 case MSM_VIDC_EXTRADATA_METADATA_LTR:
 {
 *p_extra->data = *p_extradata->data;
                p_extra->nSize = ALIGN(sizeof(OMX_OTHER_EXTRADATATYPE) + p_extradata->data_size, 4);
                p_extra->nVersion.nVersion = OMX_SPEC_VERSION;
                p_extra->nPortIndex = OMX_DirOutput;
                p_extra->eType = (OMX_EXTRADATATYPE) OMX_ExtraDataVideoLTRInfo;
                p_extra->nDataSize = p_extradata->data_size;
 break;
 }
 case MSM_VIDC_EXTRADATA_NONE:
                p_extra->nSize = ALIGN(sizeof(OMX_OTHER_EXTRADATATYPE), 4);
                p_extra->nVersion.nVersion = OMX_SPEC_VERSION;
                p_extra->nPortIndex = OMX_DirOutput;
                p_extra->eType = OMX_ExtraDataNone;
                p_extra->nDataSize = 0;
 break;
 default:
  
                DEBUG_PRINT_HIGH("Found an unrecognised extradata (%x) ignoring it",
                        p_extradata->type);
 continue;
 }

        p_extra = (OMX_OTHER_EXTRADATATYPE *)(((char *)p_extra) + p_extra->nSize);
 } while (p_extradata->type != MSM_VIDC_EXTRADATA_NONE);

  
    p_extra = (OMX_OTHER_EXTRADATATYPE *)ALIGN(p_bufhdr->pBuffer +
                p_bufhdr->nOffset + p_bufhdr->nFilledLen, 4);
 while(p_extra->eType != OMX_ExtraDataNone)
 {
        DEBUG_PRINT_LOW("[%p/%u] found extradata type %x of size %u (%u) at %p",
                p_bufhdr->pBuffer, (unsigned int)p_bufhdr->nFilledLen, p_extra->eType,
 (unsigned int)p_extra->nSize, (unsigned int)p_extra->nDataSize, p_extra);

        p_extra = (OMX_OTHER_EXTRADATATYPE *) (((OMX_U8 *) p_extra) +
                p_extra->nSize);
 }

 return true;
}
