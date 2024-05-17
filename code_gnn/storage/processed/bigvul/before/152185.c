void RenderFrameImpl::AddAutoplayFlags(const url::Origin& origin,
                                       const int32_t flags) {
  if (autoplay_flags_.first == origin) {
    autoplay_flags_.second |= flags;
  } else {
    autoplay_flags_ = std::make_pair(origin, flags);
  }
}
