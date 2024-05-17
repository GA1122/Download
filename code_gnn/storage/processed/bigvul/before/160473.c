void RenderFrameHostImpl::OnSmartClipDataExtracted(int32_t callback_id,
                                                   const base::string16& text,
                                                   const base::string16& html) {
  std::move(*smart_clip_callbacks_.Lookup(callback_id)).Run(text, html);
  smart_clip_callbacks_.Remove(callback_id);
}
