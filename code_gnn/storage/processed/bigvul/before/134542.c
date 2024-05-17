  void SetImage(const gfx::Image& image) {
    image_ = image;
    image_size_ = image.AsImageSkia().size();
  }
