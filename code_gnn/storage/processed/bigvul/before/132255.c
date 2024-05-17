void RenderFrameImpl::OnTextTrackSettingsChanged(
    const FrameMsg_TextTrackSettings_Params& params) {
  DCHECK(!frame_->parent());
  if (!render_view_->webview())
    return;

  if (params.text_tracks_enabled) {
      render_view_->webview()->settings()->setTextTrackKindUserPreference(
          WebSettings::TextTrackKindUserPreference::Captions);
  } else {
      render_view_->webview()->settings()->setTextTrackKindUserPreference(
          WebSettings::TextTrackKindUserPreference::Default);
  }
  render_view_->webview()->settings()->setTextTrackBackgroundColor(
      WebString::fromUTF8(params.text_track_background_color));
  render_view_->webview()->settings()->setTextTrackFontFamily(
      WebString::fromUTF8(params.text_track_font_family));
  render_view_->webview()->settings()->setTextTrackFontStyle(
      WebString::fromUTF8(params.text_track_font_style));
  render_view_->webview()->settings()->setTextTrackFontVariant(
      WebString::fromUTF8(params.text_track_font_variant));
  render_view_->webview()->settings()->setTextTrackTextColor(
      WebString::fromUTF8(params.text_track_text_color));
  render_view_->webview()->settings()->setTextTrackTextShadow(
      WebString::fromUTF8(params.text_track_text_shadow));
  render_view_->webview()->settings()->setTextTrackTextSize(
      WebString::fromUTF8(params.text_track_text_size));
}
