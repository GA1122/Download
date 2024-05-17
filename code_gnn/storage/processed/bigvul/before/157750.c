void WebContentsImpl::DidChangeTextSelection(const base::string16& text,
                                             const gfx::Range& range,
                                             size_t offset) {
  for (auto& observer : observers_)
    observer.DidChangeTextSelection(text, range, offset);
}
