void SimpleSoftOMXComponent::addPort(const OMX_PARAM_PORTDEFINITIONTYPE &def) {
    CHECK_EQ(def.nPortIndex, mPorts.size());

    mPorts.push();
 PortInfo *info = &mPorts.editItemAt(mPorts.size() - 1);
    info->mDef = def;
    info->mTransition = PortInfo::NONE;
}
