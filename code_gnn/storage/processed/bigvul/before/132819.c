bool PictureLayerImpl::CanHaveTilings() const {
  if (raster_source_->IsSolidColor())
    return false;
  if (!DrawsContent())
    return false;
  if (!raster_source_->HasRecordings())
    return false;
  DCHECK(!raster_source_->GetSize().IsEmpty());
  return true;
}
