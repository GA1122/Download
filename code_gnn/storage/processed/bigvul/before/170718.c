void OMXNodeInstance::addActiveBuffer(OMX_U32 portIndex, OMX::buffer_id id) {
 ActiveBuffer active;
    active.mPortIndex = portIndex;
    active.mID = id;
    mActiveBuffers.push(active);

 if (portIndex < NELEM(mNumPortBuffers)) {
 ++mNumPortBuffers[portIndex];
 }
}
