OMX::node_id OMX::makeNodeID(OMXNodeInstance *instance) {

    node_id node = (node_id)++mNodeCounter;
    mNodeIDToInstance.add(node, instance);

 return node;
}
