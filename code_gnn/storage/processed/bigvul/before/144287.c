void LoginDisplayHostWebUI::CancelUserAdding() {
  finalize_animation_type_ = ANIMATION_NONE;
  Finalize(base::OnceClosure());
}
