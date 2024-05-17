WebMediaPlayer* RenderFrameImpl::CreateWebMediaPlayerForMediaStream(
    WebMediaPlayerClient* client) {
#if defined(ENABLE_WEBRTC)
#if defined(OS_ANDROID) && defined(ARCH_CPU_ARMEL)
  bool found_neon =
      (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
  UMA_HISTOGRAM_BOOLEAN("Platform.WebRtcNEONFound", found_neon);
#endif   
  return new WebMediaPlayerMS(frame_, client, weak_factory_.GetWeakPtr(),
                              new RenderMediaLog(),
                              CreateRendererFactory());
#else
  return NULL;
#endif   
}
