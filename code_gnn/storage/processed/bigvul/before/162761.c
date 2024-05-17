void BaseRenderingContext2D::setImageSmoothingQuality(const String& quality) {
  if (quality == GetState().ImageSmoothingQuality())
    return;

  ModifiableState().SetImageSmoothingQuality(quality);
}
