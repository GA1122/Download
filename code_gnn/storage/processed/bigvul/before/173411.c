void omx_vdec::append_portdef_extradata(OMX_OTHER_EXTRADATATYPE *extra)
{
    OMX_PARAM_PORTDEFINITIONTYPE *portDefn = NULL;
    extra->nSize = OMX_PORTDEF_EXTRADATA_SIZE;
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->nPortIndex = OMX_CORE_OUTPUT_PORT_INDEX;
    extra->eType = (OMX_EXTRADATATYPE)OMX_ExtraDataPortDef;
    extra->nDataSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
    portDefn = (OMX_PARAM_PORTDEFINITIONTYPE *)(void *)extra->data;
 *portDefn = m_port_def;
    DEBUG_PRINT_LOW("append_portdef_extradata height = %u width = %u "
 "stride = %u sliceheight = %u",(unsigned int)portDefn->format.video.nFrameHeight,
 (unsigned int)portDefn->format.video.nFrameWidth,
 (unsigned int)portDefn->format.video.nStride,
 (unsigned int)portDefn->format.video.nSliceHeight);
}
