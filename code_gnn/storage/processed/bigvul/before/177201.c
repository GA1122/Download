status_t ACodec::LoadedToIdleState::allocateBuffers() {
 status_t err = mCodec->allocateBuffersOnPort(kPortIndexInput);

 if (err != OK) {
 return err;
 }

 return mCodec->allocateBuffersOnPort(kPortIndexOutput);
}
