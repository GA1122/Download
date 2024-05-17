ScopedModifyPixels::~ScopedModifyPixels() {
  if (ref_)
    ref_->texture()->OnDidModifyPixels();
}
