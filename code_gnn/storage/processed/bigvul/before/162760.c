void BaseRenderingContext2D::setImageSmoothingEnabled(bool enabled) {
  if (enabled == GetState().ImageSmoothingEnabled())
    return;

  ModifiableState().SetImageSmoothingEnabled(enabled);
}
