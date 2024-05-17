void AwContents::SmoothScroll(JNIEnv* env,
                              jobject obj,
                              jint target_x,
                              jint target_y,
                              jlong duration_ms) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  float scale = browser_view_renderer_.dip_scale() *
                browser_view_renderer_.page_scale_factor();
  render_view_host_ext_->SmoothScroll(target_x / scale, target_y / scale,
                                      duration_ms);
}
