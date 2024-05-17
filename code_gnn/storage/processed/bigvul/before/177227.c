status_t ACodec::freeBuffersOnPort(OMX_U32 portIndex) {
 status_t err = OK;
 for (size_t i = mBuffers[portIndex].size(); i > 0;) {
        i--;
 status_t err2 = freeBuffer(portIndex, i);
 if (err == OK) {
            err = err2;
 }
 }

    mDealer[portIndex].clear();
 return err;
}
