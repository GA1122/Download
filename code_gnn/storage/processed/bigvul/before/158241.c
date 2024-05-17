void RenderWidgetHostImpl::DidAllocateSharedBitmap(
    mojo::ScopedSharedBufferHandle buffer,
    const viz::SharedBitmapId& id) {
  if (!shared_bitmap_manager_->ChildAllocatedSharedBitmap(std::move(buffer),
                                                          id)) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RWH_SHARED_BITMAP);
  }
  owned_bitmaps_.insert(id);
}
