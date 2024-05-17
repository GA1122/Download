status_t OMX::configureVideoTunnelMode(
        node_id node, OMX_U32 portIndex, OMX_BOOL tunneled,
        OMX_U32 audioHwSync, native_handle_t **sidebandHandle) {
 return findInstance(node)->configureVideoTunnelMode(
            portIndex, tunneled, audioHwSync, sidebandHandle);
}
