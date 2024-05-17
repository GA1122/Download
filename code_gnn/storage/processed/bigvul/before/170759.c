status_t OMXNodeInstance::storeMetaDataInBuffers(
        OMX_U32 portIndex,
        OMX_BOOL enable) {
 Mutex::Autolock autolock(mLock);
    CLOG_CONFIG(storeMetaDataInBuffers, "%s:%u en:%d", portString(portIndex), portIndex, enable);
 return storeMetaDataInBuffers_l(
            portIndex, enable,
            OMX_FALSE  , NULL  );
}
