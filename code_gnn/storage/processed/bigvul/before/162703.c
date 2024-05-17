void BaseRenderingContext2D::RestoreMatrixClipStack(PaintCanvas* c) const {
  if (!c)
    return;
  HeapVector<Member<CanvasRenderingContext2DState>>::const_iterator curr_state;
  DCHECK(state_stack_.begin() < state_stack_.end());
  for (curr_state = state_stack_.begin(); curr_state < state_stack_.end();
       curr_state++) {
    c->setMatrix(SkMatrix::I());
    if (curr_state->Get()) {
      curr_state->Get()->PlaybackClips(c);
      c->setMatrix(AffineTransformToSkMatrix(curr_state->Get()->Transform()));
    }
    c->save();
  }
  c->restore();
  ValidateStateStack();
}
