  void GetThumbnailSize(int width, int height, gfx::Size* output) const {
    float thumbnail_width = static_cast<float>(aeropeek_size_.width());
    float thumbnail_height = static_cast<float>(aeropeek_size_.height());
    float source_width = static_cast<float>(width);
    float source_height = static_cast<float>(height);
    DCHECK(source_width && source_height);

    float ratio_width = thumbnail_width / source_width;
    float ratio_height = thumbnail_height / source_height;
    if (ratio_width > ratio_height) {
      thumbnail_width = source_width * ratio_height;
    } else {
      thumbnail_height = source_height * ratio_width;
    }

    output->set_width(static_cast<int>(thumbnail_width));
    output->set_height(static_cast<int>(thumbnail_height));
  }
