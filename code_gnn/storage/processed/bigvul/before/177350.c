void ACodec::waitUntilAllPossibleNativeWindowBuffersAreReturnedToUs() {
 if (mNativeWindow == NULL) {
 return;
 }

 while (countBuffersOwnedByNativeWindow() > mNumUndequeuedBuffers
 && dequeueBufferFromNativeWindow() != NULL) {
 if (storingMetadataInDecodedBuffers() && mMetadataBuffersToSubmit > 0) {
 --mMetadataBuffersToSubmit;
 }
 }
}
