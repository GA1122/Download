void WebGL2RenderingContextBase::pauseTransformFeedback() {
  if (isContextLost())
    return;

  ContextGL()->PauseTransformFeedback();
}
