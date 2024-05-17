void OMXNodeInstance::bumpDebugLevel_l(size_t numInputBuffers, size_t numOutputBuffers) {
 if (DEBUG == ADebug::kDebugInternalState) {
        DEBUG_BUMP = ADebug::kDebugAll;
 if (numInputBuffers > 0) {
            mDebugLevelBumpPendingBuffers[kPortIndexInput] = numInputBuffers;
 }
 if (numOutputBuffers > 0) {
            mDebugLevelBumpPendingBuffers[kPortIndexOutput] = numOutputBuffers;
 }
 }
}
