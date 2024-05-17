GURL RenderViewImpl::GetOpenerUrl() const {
  if (opener_id_ == MSG_ROUTING_NONE || opener_suppressed_)
    return GURL();
  else
    return creator_url_;
}
