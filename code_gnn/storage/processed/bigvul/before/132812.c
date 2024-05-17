PictureLayerTiling* PictureLayerImpl::AddTiling(float contents_scale) {
  DCHECK(CanHaveTilingWithScale(contents_scale)) <<
      "contents_scale: " << contents_scale;

  PictureLayerTiling* tiling =
      tilings_->AddTiling(contents_scale, raster_source_->GetSize());

  DCHECK(raster_source_->HasRecordings());

  return tiling;
}
