void SimpleSoftOMXComponent::onPortFlush(
        OMX_U32 portIndex, bool sendFlushComplete) {
 if (portIndex == OMX_ALL) {
 for (size_t i = 0; i < mPorts.size(); ++i) {
            onPortFlush(i, sendFlushComplete);
 }

 if (sendFlushComplete) {
            notify(OMX_EventCmdComplete, OMX_CommandFlush, OMX_ALL, NULL);
 }

 return;
 }

    CHECK_LT(portIndex, mPorts.size());

 PortInfo *port = &mPorts.editItemAt(portIndex);
 if (port->mTransition != PortInfo::NONE) {
        notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 }

 for (size_t i = 0; i < port->mBuffers.size(); ++i) {
 BufferInfo *buffer = &port->mBuffers.editItemAt(i);

 if (!buffer->mOwnedByUs) {
 continue;
 }

        buffer->mHeader->nFilledLen = 0;
        buffer->mHeader->nOffset = 0;
        buffer->mHeader->nFlags = 0;

        buffer->mOwnedByUs = false;

 if (port->mDef.eDir == OMX_DirInput) {
            notifyEmptyBufferDone(buffer->mHeader);
 } else {
            CHECK_EQ(port->mDef.eDir, OMX_DirOutput);

            notifyFillBufferDone(buffer->mHeader);
 }
 }

    port->mQueue.clear();

 if (sendFlushComplete) {
        notify(OMX_EventCmdComplete, OMX_CommandFlush, portIndex, NULL);

        onPortFlushCompleted(portIndex);
 }
}
