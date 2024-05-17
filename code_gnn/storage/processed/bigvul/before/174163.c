status_t OMX::sendCommand(
        node_id node, OMX_COMMANDTYPE cmd, OMX_S32 param) {
 return findInstance(node)->sendCommand(cmd, param);
}
