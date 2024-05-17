void BaseRenderingContext2D::save() {
  state_stack_.back()->Save();
}
