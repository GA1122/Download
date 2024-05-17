void WebContentsImpl::NotifyViewportFitChanged(
    blink::mojom::ViewportFit value) {
  for (auto& observer : observers_)
    observer.ViewportFitChanged(value);
}
