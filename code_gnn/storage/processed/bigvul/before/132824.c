scoped_ptr<TilingSetRasterQueue> PictureLayerImpl::CreateRasterQueue(
    bool prioritize_low_res) {
  if (!tilings_)
    return make_scoped_ptr(new TilingSetRasterQueue());
  return make_scoped_ptr(
      new TilingSetRasterQueue(tilings_.get(), prioritize_low_res));
}
