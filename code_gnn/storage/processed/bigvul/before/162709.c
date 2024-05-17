void BaseRenderingContext2D::UnwindStateStack() {
  if (size_t stack_size = state_stack_.size()) {
    if (PaintCanvas* sk_canvas = ExistingDrawingCanvas()) {
      while (--stack_size)
        sk_canvas->restore();
    }
  }
}
