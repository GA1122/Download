void omx_vdec::append_mpeg2_seqdisplay_extradata(OMX_OTHER_EXTRADATATYPE *extra,
 struct msm_vidc_mpeg2_seqdisp_payload *seq_display_payload)
{
    OMX_QCOM_EXTRADATA_MPEG2SEQDISPLAY *seq_display = NULL;
    extra->nSize = OMX_MPEG2SEQDISP_EXTRADATA_SIZE;
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataMpeg2SeqDisplay;
    extra->nDataSize = sizeof(OMX_QCOM_EXTRADATA_MPEG2SEQDISPLAY);
    seq_display = (OMX_QCOM_EXTRADATA_MPEG2SEQDISPLAY *)(void *)extra->data;
    seq_display->disp_width = seq_display_payload->disp_width;
    seq_display->disp_height = seq_display_payload->disp_height;
    print_debug_extradata(extra);
}
