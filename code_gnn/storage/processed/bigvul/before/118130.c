void WebContentsAndroid::DidStartNavigationTransitionForFrame(int64 frame_id) {
  JNIEnv* env = AttachCurrentThread();
  Java_WebContentsImpl_didStartNavigationTransitionForFrame(
      env, obj_.obj(), frame_id);
}
