void BaseRenderingContext2D::restore() {
  ValidateStateStack();
  if (GetState().HasUnrealizedSaves()) {
    state_stack_.back()->Restore();
    return;
  }
  DCHECK_GE(state_stack_.size(), 1u);
  if (state_stack_.size() <= 1)
    return;
  path_.Transform(GetState().Transform());
  state_stack_.pop_back();
  state_stack_.back()->ClearResolvedFilter();
  path_.Transform(GetState().Transform().Inverse());
  PaintCanvas* c = DrawingCanvas();
  if (c)
    c->restore();

  ValidateStateStack();
}
