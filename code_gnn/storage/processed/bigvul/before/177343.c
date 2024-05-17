void ACodec::ExecutingState::submitOutputBuffers() {
    submitRegularOutputBuffers();
 if (mCodec->storingMetadataInDecodedBuffers()) {
        submitOutputMetaBuffers();
 }
}
