void omx_vdec::append_framepack_extradata(OMX_OTHER_EXTRADATATYPE *extra,
 struct msm_vidc_s3d_frame_packing_payload *s3d_frame_packing_payload)
{
    OMX_QCOM_FRAME_PACK_ARRANGEMENT *framepack;
 if (FRAME_PACK_SIZE*sizeof(OMX_U32) != sizeof(struct msm_vidc_s3d_frame_packing_payload)) {
        DEBUG_PRINT_ERROR("frame packing size mismatch");
 return;
 }
    extra->nSize = OMX_FRAMEPACK_EXTRADATA_SIZE;
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataFramePackingArrangement;
    extra->nDataSize = sizeof(OMX_QCOM_FRAME_PACK_ARRANGEMENT);
    framepack = (OMX_QCOM_FRAME_PACK_ARRANGEMENT *)(void *)extra->data;
    framepack->nSize = sizeof(OMX_QCOM_FRAME_PACK_ARRANGEMENT);
    framepack->nVersion.nVersion = OMX_SPEC_VERSION;
    framepack->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    memcpy(&framepack->id, s3d_frame_packing_payload,
 sizeof(struct msm_vidc_s3d_frame_packing_payload));
    memcpy(&m_frame_pack_arrangement, framepack,
 sizeof(OMX_QCOM_FRAME_PACK_ARRANGEMENT));
    print_debug_extradata(extra);
}
