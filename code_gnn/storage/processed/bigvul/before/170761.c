void OMXNodeInstance::unbumpDebugLevel_l(size_t portIndex) {
 if (mDebugLevelBumpPendingBuffers[portIndex]) {
 --mDebugLevelBumpPendingBuffers[portIndex];
 }
 if (!mDebugLevelBumpPendingBuffers[0]
 && !mDebugLevelBumpPendingBuffers[1]) {
        DEBUG_BUMP = DEBUG;
 }
}
