void PictureLayerImpl::GetContentsResourceId(
    ResourceProvider::ResourceId* resource_id,
    gfx::Size* resource_size) const {
  DCHECK_IMPLIES(!raster_source_->GetSize().IsEmpty(),
                 bounds() == raster_source_->GetSize())
      << " bounds " << bounds().ToString() << " pile "
      << raster_source_->GetSize().ToString();
  gfx::Rect content_rect(bounds());
  PictureLayerTilingSet::CoverageIterator iter(
      tilings_.get(), 1.f, content_rect, ideal_contents_scale_);

  if (!iter || !*iter) {
    *resource_id = 0;
    return;
  }

  DCHECK(iter.geometry_rect() == content_rect)
      << "iter rect " << iter.geometry_rect().ToString() << " content rect "
      << content_rect.ToString();

  const ManagedTileState::DrawInfo& draw_info = iter->draw_info();
  if (!draw_info.IsReadyToDraw() ||
      draw_info.mode() != ManagedTileState::DrawInfo::RESOURCE_MODE) {
    *resource_id = 0;
    return;
  }

  *resource_id = draw_info.get_resource_id();
  *resource_size = iter.texture_size();
}
