void Browser::ToggleSpeechInput() {
  GetSelectedTabContentsWrapper()->render_view_host()->ToggleSpeechInput();
}
