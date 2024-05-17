void WebGLRenderingContextBase::RestoreClearColor() {
  if (isContextLost())
    return;

  ContextGL()->ClearColor(clear_color_[0], clear_color_[1], clear_color_[2],
                          clear_color_[3]);
}
