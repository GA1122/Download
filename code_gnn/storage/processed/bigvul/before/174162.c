status_t OMX::prepareForAdaptivePlayback(
        node_id node, OMX_U32 portIndex, OMX_BOOL enable,
        OMX_U32 maxFrameWidth, OMX_U32 maxFrameHeight) {
 return findInstance(node)->prepareForAdaptivePlayback(
            portIndex, enable, maxFrameWidth, maxFrameHeight);
}
