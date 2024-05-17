ScopedPixelUnpackState::ScopedPixelUnpackState(ContextState* state)
    : state_(state) {
  DCHECK(state_);
  state_->PushTextureUnpackState();
}
