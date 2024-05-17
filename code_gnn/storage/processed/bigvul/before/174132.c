OMX_ERRORTYPE OMX::OnEmptyBufferDone(
        node_id node, buffer_id buffer, OMX_IN OMX_BUFFERHEADERTYPE *pBuffer, int fenceFd) {
    ALOGV("OnEmptyBufferDone buffer=%p", pBuffer);

    omx_message msg;
    msg.type = omx_message::EMPTY_BUFFER_DONE;
    msg.node = node;
    msg.fenceFd = fenceFd;
    msg.u.buffer_data.buffer = buffer;

    findDispatcher(node)->post(msg);

 return OMX_ErrorNone;
}
