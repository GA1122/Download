skia::RefPtr<SkPicture> PictureLayerImpl::GetPicture() {
  return raster_source_->GetFlattenedPicture();
}
