void omx_vdec::append_bitsinfo_extradata(OMX_OTHER_EXTRADATATYPE *extra,
 struct msm_vidc_frame_bits_info_payload *bits_payload)
{
    OMX_QCOM_EXTRADATA_BITS_INFO * bits = NULL;
 if (!bits_payload) {
        DEBUG_PRINT_ERROR("bits info payload is NULL");
 return;
 }
    extra->nSize = OMX_BITSINFO_EXTRADATA_SIZE;
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataInputBitsInfo;
    extra->nDataSize = sizeof(OMX_QCOM_EXTRADATA_BITS_INFO);
    bits = (OMX_QCOM_EXTRADATA_BITS_INFO*)(void *)extra->data;
    bits->frame_bits = bits_payload->frame_bits;
    bits->header_bits = bits_payload->header_bits;
    print_debug_extradata(extra);
}
