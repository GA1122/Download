void WebLocalFrameImpl::SetEngagementLevel(mojom::EngagementLevel level) {
  GetFrame()->GetDocument()->SetEngagementLevel(level);
}
