ScopedTextureBinder::ScopedTextureBinder(ContextState* state,
                                         ErrorState* error_state,
                                         GLuint id,
                                         GLenum target)
    : state_(state), error_state_(error_state), target_(target) {
  ScopedGLErrorSuppressor suppressor("ScopedTextureBinder::ctor", error_state_);

  auto* api = state->api();
  api->glActiveTextureFn(GL_TEXTURE0);
  api->glBindTextureFn(target, id);
}
