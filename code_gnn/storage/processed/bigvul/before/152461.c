void RenderFrameImpl::OnTextTrackSettingsChanged(
    const FrameMsg_TextTrackSettings_Params& params) {
  DCHECK(!frame_->Parent());
  if (!render_view_->webview())
    return;

  if (params.text_tracks_enabled) {
    render_view_->webview()->GetSettings()->SetTextTrackKindUserPreference(
        WebSettings::TextTrackKindUserPreference::kCaptions);
  } else {
    render_view_->webview()->GetSettings()->SetTextTrackKindUserPreference(
        WebSettings::TextTrackKindUserPreference::kDefault);
  }
  render_view_->webview()->GetSettings()->SetTextTrackBackgroundColor(
      WebString::FromUTF8(params.text_track_background_color));
  render_view_->webview()->GetSettings()->SetTextTrackFontFamily(
      WebString::FromUTF8(params.text_track_font_family));
  render_view_->webview()->GetSettings()->SetTextTrackFontStyle(
      WebString::FromUTF8(params.text_track_font_style));
  render_view_->webview()->GetSettings()->SetTextTrackFontVariant(
      WebString::FromUTF8(params.text_track_font_variant));
  render_view_->webview()->GetSettings()->SetTextTrackTextColor(
      WebString::FromUTF8(params.text_track_text_color));
  render_view_->webview()->GetSettings()->SetTextTrackTextShadow(
      WebString::FromUTF8(params.text_track_text_shadow));
  render_view_->webview()->GetSettings()->SetTextTrackTextSize(
      WebString::FromUTF8(params.text_track_text_size));
}
