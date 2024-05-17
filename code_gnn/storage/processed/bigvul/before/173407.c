void omx_vdec::append_frame_dimension_extradata(OMX_OTHER_EXTRADATATYPE *extra)
{
    OMX_QCOM_EXTRADATA_FRAMEDIMENSION *frame_dimension;
 if (!(client_extradata & OMX_FRAMEDIMENSION_EXTRADATA)) {
 return;
 }
    extra->nSize = OMX_FRAMEDIMENSION_EXTRADATA_SIZE;
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataFrameDimension;
    extra->nDataSize = sizeof(OMX_QCOM_EXTRADATA_FRAMEDIMENSION);
    frame_dimension = (OMX_QCOM_EXTRADATA_FRAMEDIMENSION *)(void *)extra->data;
    frame_dimension->nDecWidth = rectangle.nLeft;
    frame_dimension->nDecHeight = rectangle.nTop;
    frame_dimension->nActualWidth = rectangle.nWidth;
    frame_dimension->nActualHeight = rectangle.nHeight;
}
