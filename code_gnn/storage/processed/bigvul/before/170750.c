void OMXNodeInstance::removeActiveBuffer(
        OMX_U32 portIndex, OMX::buffer_id id) {
 for (size_t i = 0; i < mActiveBuffers.size(); ++i) {
 if (mActiveBuffers[i].mPortIndex == portIndex
 && mActiveBuffers[i].mID == id) {
            mActiveBuffers.removeItemsAt(i);

 if (portIndex < NELEM(mNumPortBuffers)) {
 --mNumPortBuffers[portIndex];
 }
 return;
 }
 }

     CLOGW("Attempt to remove an active buffer [%#x] we know nothing about...", id);
}
