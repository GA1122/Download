void omx_vdec::append_qp_extradata(OMX_OTHER_EXTRADATATYPE *extra,
 struct msm_vidc_frame_qp_payload *qp_payload)
{
    OMX_QCOM_EXTRADATA_QP * qp = NULL;
 if (!qp_payload) {
        DEBUG_PRINT_ERROR("QP payload is NULL");
 return;
 }
    extra->nSize = OMX_QP_EXTRADATA_SIZE;
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataQP;
    extra->nDataSize = sizeof(OMX_QCOM_EXTRADATA_QP);
    qp = (OMX_QCOM_EXTRADATA_QP *)(void *)extra->data;
    qp->nQP = qp_payload->frame_qp;
    print_debug_extradata(extra);
}
