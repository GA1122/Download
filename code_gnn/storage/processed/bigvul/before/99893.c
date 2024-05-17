void WebPluginDelegateProxy::ResetWindowlessBitmaps() {
#if defined(OS_MACOSX)
  if (backing_store_.get()) {
    int ack_key = backing_store_->handle().fd;

    DCHECK_NE(ack_key, -1);

    DCHECK(old_transport_dibs_.find(ack_key) == old_transport_dibs_.end());

    RelatedTransportDIBs old_dibs;
    old_dibs.backing_store =
        linked_ptr<TransportDIB>(backing_store_.release());
    old_dibs.transport_store =
        linked_ptr<TransportDIB>(transport_store_.release());
    old_dibs.background_store =
        linked_ptr<TransportDIB>(background_store_.release());

    old_transport_dibs_[ack_key] = old_dibs;
  } else {
    DCHECK(!transport_store_.get());
    DCHECK(!background_store_.get());
  }
#else
  backing_store_.reset();
  transport_store_.reset();
  background_store_.reset();
#endif

  backing_store_canvas_.reset();
  transport_store_canvas_.reset();
  background_store_canvas_.release();
  backing_store_painted_ = gfx::Rect();
}
