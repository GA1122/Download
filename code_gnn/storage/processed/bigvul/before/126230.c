void Browser::ResizeDueToAutoResize(WebContents* source,
                                    const gfx::Size& new_size) {
  window_->ResizeDueToAutoResize(source, new_size);
}
