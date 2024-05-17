void OMXNodeInstance::freeActiveBuffers() {
 for (size_t i = mActiveBuffers.size(); i > 0;) {
        i--;
        freeBuffer(mActiveBuffers[i].mPortIndex, mActiveBuffers[i].mID);
 }
}
