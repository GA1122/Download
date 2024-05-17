jboolean WebContentsAndroid::IsRenderWidgetHostViewReady(
    JNIEnv* env,
    jobject obj) {
  RenderWidgetHostViewAndroid* view = GetRenderWidgetHostViewAndroid();
  return view && view->HasValidFrame();
}
