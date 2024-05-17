void RenderWidgetHostImpl::OnMsgShowDisambiguationPopup(
    const gfx::Rect& rect,
    const gfx::Size& size,
    const TransportDIB::Id& id) {
  DCHECK(!rect.IsEmpty());
  DCHECK(!size.IsEmpty());

  TransportDIB* dib = process_->GetTransportDIB(id);
  DCHECK(dib->memory());
  DCHECK(dib->size() == SkBitmap::ComputeSize(SkBitmap::kARGB_8888_Config,
                                              size.width(), size.height()));

  SkBitmap zoomed_bitmap;
  zoomed_bitmap.setConfig(SkBitmap::kARGB_8888_Config,
      size.width(), size.height());
  zoomed_bitmap.setPixels(dib->memory());

#if defined(OS_ANDROID)
  if (view_)
    view_->ShowDisambiguationPopup(rect, zoomed_bitmap);
#else
  NOTIMPLEMENTED();
#endif

  zoomed_bitmap.setPixels(0);
  Send(new ViewMsg_ReleaseDisambiguationPopupDIB(GetRoutingID(),
                                                 dib->handle()));
}
