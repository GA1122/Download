OMX_ERRORTYPE SoftVideoDecoderOMXComponent::getExtensionIndex(
 const char *name, OMX_INDEXTYPE *index) {
 if (!strcmp(name, "OMX.google.android.index.prepareForAdaptivePlayback")) {
 *(int32_t*)index = kPrepareForAdaptivePlaybackIndex;
 return OMX_ErrorNone;
 }

 return SimpleSoftOMXComponent::getExtensionIndex(name, index);
}
