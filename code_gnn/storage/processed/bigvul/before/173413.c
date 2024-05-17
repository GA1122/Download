void omx_vdec::append_terminator_extradata(OMX_OTHER_EXTRADATATYPE *extra)
{
 if (!client_extradata) {
 return;
 }
    extra->nSize = sizeof(OMX_OTHER_EXTRADATATYPE);
    extra->nVersion.nVersion = OMX_SPEC_VERSION;
    extra->eType = OMX_ExtraDataNone;
    extra->nDataSize = 0;
    extra->data[0] = 0;

    print_debug_extradata(extra);
}
