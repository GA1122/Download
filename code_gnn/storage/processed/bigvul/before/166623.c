void WebGL2RenderingContextBase::useProgram(WebGLProgram* program) {
  if (transform_feedback_binding_->active() &&
      !transform_feedback_binding_->paused()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "useProgram",
                      "transform feedback is active and not paused");
    return;
  }
  WebGLRenderingContextBase::useProgram(program);
}
