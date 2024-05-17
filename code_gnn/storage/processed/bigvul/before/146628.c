void DrawingBuffer::MailboxReleasedSoftware(
    std::unique_ptr<viz::SharedBitmap> bitmap,
    const IntSize& size,
    const gpu::SyncToken& sync_token,
    bool lost_resource) {
  DCHECK(!sync_token.HasData());   
  if (destruction_in_progress_ || lost_resource || is_hidden_ || size != size_)
    return;   

  RecycledBitmap recycled = {std::move(bitmap), size_};
  recycled_bitmaps_.push_back(std::move(recycled));
}
