void WebGLRenderingContextBase::RestoreScissorBox() {
  if (isContextLost())
    return;

  ContextGL()->Scissor(scissor_box_[0], scissor_box_[1], scissor_box_[2],
                       scissor_box_[3]);
}
