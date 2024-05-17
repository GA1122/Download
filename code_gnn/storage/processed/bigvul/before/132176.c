bool RenderFrameImpl::AreSecureCodecsSupported() {
#if defined(OS_ANDROID)
  return render_view_->renderer_preferences_
      .use_video_overlay_for_embedded_encrypted_video;
#else
  return false;
#endif   
}
