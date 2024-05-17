std::unique_ptr<viz::SharedBitmap> DrawingBuffer::CreateOrRecycleBitmap() {
  auto it = std::remove_if(
      recycled_bitmaps_.begin(), recycled_bitmaps_.end(),
      [this](const RecycledBitmap& bitmap) { return bitmap.size != size_; });
  recycled_bitmaps_.Shrink(it - recycled_bitmaps_.begin());

  if (!recycled_bitmaps_.IsEmpty()) {
    RecycledBitmap recycled = std::move(recycled_bitmaps_.back());
    recycled_bitmaps_.pop_back();
    DCHECK(recycled.size == size_);
    return std::move(recycled.bitmap);
  }

  return Platform::Current()->AllocateSharedBitmap(size_);
}
