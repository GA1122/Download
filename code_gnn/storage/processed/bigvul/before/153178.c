void Compositor::SetDisplayColorMatrix(const SkMatrix44& matrix) {
  display_color_matrix_ = matrix;
  if (context_factory_private_)
    context_factory_private_->SetDisplayColorMatrix(this, matrix);
}
