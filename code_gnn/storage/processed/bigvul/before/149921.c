void LayerTreeHostImpl::CreateUIResource(UIResourceId uid,
                                         const UIResourceBitmap& bitmap) {
  DCHECK_GT(uid, 0);

  ResourceId id = ResourceIdForUIResource(uid);
  if (id)
    DeleteUIResource(uid);

  if (!has_valid_compositor_frame_sink_) {
    evicted_ui_resources_.insert(uid);
    return;
  }

  ResourceFormat format = resource_provider_->best_texture_format();
  switch (bitmap.GetFormat()) {
    case UIResourceBitmap::RGBA8:
      break;
    case UIResourceBitmap::ALPHA_8:
      format = ALPHA_8;
      break;
    case UIResourceBitmap::ETC1:
      format = ETC1;
      break;
  }

  const gfx::Size source_size = bitmap.GetSize();
  gfx::Size upload_size = bitmap.GetSize();
  bool scaled = false;

  int max_texture_size = resource_provider_->max_texture_size();
  if (source_size.width() > max_texture_size ||
      source_size.height() > max_texture_size) {
    scaled = true;
    int edge = std::max(source_size.width(), source_size.height());
    float scale = static_cast<float>(max_texture_size - 1) / edge;
    DCHECK_LT(scale, 1.f);
    upload_size = gfx::ScaleToCeiledSize(source_size, scale, scale);
  }

  id = resource_provider_->CreateResource(
      upload_size, ResourceProvider::TEXTURE_HINT_IMMUTABLE, format,
      gfx::ColorSpace::CreateSRGB());

  if (!scaled) {
    AutoLockUIResourceBitmap bitmap_lock(bitmap);
    auto* pixels = bitmap_lock.GetPixels();
    resource_provider_->CopyToResource(id, pixels, source_size);
  } else {
    DCHECK_EQ(bitmap.GetFormat(), UIResourceBitmap::RGBA8);

    float canvas_scale_x =
        upload_size.width() / static_cast<float>(source_size.width());
    float canvas_scale_y =
        upload_size.height() / static_cast<float>(source_size.height());

    SkImageInfo info = SkImageInfo::MakeN32(
        source_size.width(), source_size.height(), kPremul_SkAlphaType);
    int row_bytes = source_size.width() * 4;

    AutoLockUIResourceBitmap bitmap_lock(bitmap);
    SkBitmap source_bitmap;
    source_bitmap.setInfo(info, row_bytes);
    source_bitmap.setPixels(const_cast<uint8_t*>(bitmap_lock.GetPixels()));

    SkBitmap scaled_bitmap;
    scaled_bitmap.allocN32Pixels(upload_size.width(), upload_size.height());
    SkCanvas scaled_canvas(scaled_bitmap);
    scaled_canvas.scale(canvas_scale_x, canvas_scale_y);
    scaled_canvas.clear(SK_ColorTRANSPARENT);
    scaled_canvas.drawBitmap(source_bitmap, 0, 0);

    SkAutoLockPixels scaled_bitmap_lock(scaled_bitmap);
    auto* pixels = static_cast<uint8_t*>(scaled_bitmap.getPixels());
    resource_provider_->CopyToResource(id, pixels, upload_size);
  }

  UIResourceData data;
  data.resource_id = id;
  data.opaque = bitmap.GetOpaque();
  ui_resource_map_[uid] = data;

  resource_provider_->GenerateSyncTokenForResource(id);
  MarkUIResourceNotEvicted(uid);
}
