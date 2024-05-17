void RenderWidgetHostImpl::OnShowDisambiguationPopup(
    const gfx::Rect& rect_pixels,
    const gfx::Size& size,
    base::SharedMemoryHandle handle) {
  DCHECK(!rect_pixels.IsEmpty());
  DCHECK(!size.IsEmpty());

  SkImageInfo info = SkImageInfo::MakeN32Premul(size.width(), size.height());
  size_t shm_size = info.computeMinByteSize();

  base::SharedMemory shm(handle, false  );
  if (shm_size == 0 || !shm.Map(shm_size)) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RWH_SHARED_BITMAP);
    return;
  }

  SkBitmap zoomed_bitmap;
  zoomed_bitmap.installPixels(info, shm.memory(), info.minRowBytes());

  if (view_)
    view_->ShowDisambiguationPopup(rect_pixels, zoomed_bitmap);

}
