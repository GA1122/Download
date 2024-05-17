void WebPluginDelegateProxy::ResetWindowlessBitmaps() {
#if defined(OS_MACOSX)
  DCHECK(!background_store_.get());
  if (transport_store_.get()) {
    int ack_key = transport_store_->handle().fd;

    DCHECK_NE(ack_key, -1);

    DCHECK(old_transport_dibs_.find(ack_key) == old_transport_dibs_.end());

    old_transport_dibs_[ack_key] =
        linked_ptr<TransportDIB>(transport_store_.release());
  }
#else
  transport_store_.reset();
  background_store_.reset();
#endif
#if defined(OS_WIN)
  backing_store_.reset();
#else
  backing_store_.resize(0);
#endif

  backing_store_canvas_.reset();
  transport_store_canvas_.reset();
  background_store_canvas_.reset();
  backing_store_painted_ = gfx::Rect();
}
