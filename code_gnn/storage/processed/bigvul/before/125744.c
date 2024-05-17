void TabSpecificContentSettings::DidStartProvisionalLoadForFrame(
    int64 frame_id,
    int64 parent_frame_id,
    bool is_main_frame,
    const GURL& validated_url,
    bool is_error_page,
    bool is_iframe_srcdoc,
    RenderViewHost* render_view_host) {
  if (!is_main_frame)
    return;

  if (!is_error_page)
    ClearCookieSpecificContentSettings();
  ClearGeolocationContentSettings();
}
