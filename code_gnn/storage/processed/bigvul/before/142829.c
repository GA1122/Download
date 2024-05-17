bool HTMLMediaElement::LoadIsDeferred() const {
  return deferred_load_state_ != kNotDeferred;
}
