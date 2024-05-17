void OMX::invalidateNodeID_l(node_id node) {
    mNodeIDToInstance.removeItem(node);
}
