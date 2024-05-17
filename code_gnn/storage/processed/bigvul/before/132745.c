void ChromotingInstance::OnVideoSize(const webrtc::DesktopSize& size,
                                        const webrtc::DesktopVector& dpi) {
  mouse_input_filter_.set_output_size(size);

  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetInteger("width", size.width());
  data->SetInteger("height", size.height());
  if (dpi.x())
    data->SetInteger("x_dpi", dpi.x());
  if (dpi.y())
    data->SetInteger("y_dpi", dpi.y());
  PostLegacyJsonMessage("onDesktopSize", data.Pass());
}
