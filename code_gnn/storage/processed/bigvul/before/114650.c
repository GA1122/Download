void WebPluginDelegateProxy::ResetWindowlessBitmaps() {
  transport_stores_[0].dib.reset();
  transport_stores_[1].dib.reset();
  background_store_.dib.reset();

  transport_stores_[0].canvas.reset();
  transport_stores_[1].canvas.reset();
  background_store_.canvas.reset();
  transport_store_painted_ = gfx::Rect();
  front_buffer_diff_ = gfx::Rect();
}
