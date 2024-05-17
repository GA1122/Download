void BaseRenderingContext2D::RealizeSaves() {
  ValidateStateStack();
  if (GetState().HasUnrealizedSaves()) {
    DCHECK_GE(state_stack_.size(), 1u);
    state_stack_.back()->Restore();
    state_stack_.push_back(CanvasRenderingContext2DState::Create(
        GetState(), CanvasRenderingContext2DState::kDontCopyClipList));
    state_stack_.back()->ResetUnrealizedSaveCount();
    PaintCanvas* canvas = DrawingCanvas();
    if (canvas)
      canvas->save();
    ValidateStateStack();
  }
}
