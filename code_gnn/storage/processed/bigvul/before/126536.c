bool TabStripGtk::IsCompatibleWith(TabStripGtk* other) {
  return model_->profile() == other->model()->profile();
}
