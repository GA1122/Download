status_t ACodec::setMinBufferSize(OMX_U32 portIndex, size_t size) {
    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = portIndex;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
 return err;
 }

 if (def.nBufferSize >= size) {
 return OK;
 }

    def.nBufferSize = size;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
 return err;
 }

    err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
 return err;
 }

 if (def.nBufferSize < size) {
        ALOGE("failed to set min buffer size to %zu (is still %u)", size, def.nBufferSize);
 return FAILED_TRANSACTION;
 }

 return OK;
}
