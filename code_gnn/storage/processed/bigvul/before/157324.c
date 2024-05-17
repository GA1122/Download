void WebMediaPlayerImpl::SetPoster(const blink::WebURL& poster) {
  has_poster_ = !poster.IsEmpty();
#if defined(OS_ANDROID)   
  cast_impl_.setPoster(poster);
#endif   
}
