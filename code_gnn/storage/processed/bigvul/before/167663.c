static void SetRuntimeFeatureDefaultsForPlatform() {
#if defined(OS_ANDROID)
  WebRuntimeFeatures::EnablePagePopup(false);
  WebRuntimeFeatures::EnableInputMultipleFieldsUI(false);
  WebRuntimeFeatures::EnableSharedWorker(false);
  WebRuntimeFeatures::EnableNavigatorContentUtils(false);
  WebRuntimeFeatures::EnableOrientationEvent(true);
  WebRuntimeFeatures::EnableFastMobileScrolling(true);
  WebRuntimeFeatures::EnableMediaCapture(true);
  WebRuntimeFeatures::EnableNotificationConstructor(false);
  WebRuntimeFeatures::EnableAudioOutputDevices(false);
  WebRuntimeFeatures::EnableOnDeviceChange(false);
  WebRuntimeFeatures::EnableMediaSession(true);
  WebRuntimeFeatures::EnableMediaControlsOverlayPlayButton(true);
  WebRuntimeFeatures::EnableRemotePlaybackBackend(true);
  WebRuntimeFeatures::EnablePictureInPictureAPI(false);
#else   
  WebRuntimeFeatures::EnableNavigatorContentUtils(true);
  WebRuntimeFeatures::EnableRemotePlaybackBackend(false);
#endif   

#if defined(OS_ANDROID) || defined(USE_AURA)
  WebRuntimeFeatures::EnableCompositedSelectionUpdate(true);
#endif

#if !(defined OS_ANDROID || defined OS_CHROMEOS)
  WebRuntimeFeatures::EnableNetInfoDownlinkMax(false);
#endif

#if defined(OS_CHROMEOS) || defined(OS_ANDROID) || defined(OS_MACOSX)
  WebRuntimeFeatures::EnableWebBluetooth(true);
#endif
#if defined(OS_WIN)
  if (base::win::GetVersion() >= base::win::VERSION_WIN10)
    WebRuntimeFeatures::EnableWebBluetooth(true);
#endif

#if defined(OS_CHROMEOS)
  WebRuntimeFeatures::EnableForceTallerSelectPopup(true);
#endif

#if !defined(OS_MACOSX)
  WebRuntimeFeatures::EnableNotificationContentImage(true);
#endif
}
