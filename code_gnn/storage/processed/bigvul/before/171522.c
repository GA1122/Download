SimpleSoftOMXComponent::PortInfo *SimpleSoftOMXComponent::editPortInfo(
        OMX_U32 portIndex) {
    CHECK_LT(portIndex, mPorts.size());
 return &mPorts.editItemAt(portIndex);
}
