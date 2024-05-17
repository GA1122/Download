ACodec::BaseState::BaseState(ACodec *codec, const sp<AState> &parentState)
 : AState(parentState),
      mCodec(codec) {
}
