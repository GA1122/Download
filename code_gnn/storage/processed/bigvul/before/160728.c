void RenderFrameImpl::SetEngagementLevel(const url::Origin& origin,
                                         blink::mojom::EngagementLevel level) {
  if (frame_ && url::Origin(frame_->GetSecurityOrigin()) == origin) {
    frame_->SetEngagementLevel(level);
    return;
  }

  engagement_level_ = std::make_pair(origin, level);
}
