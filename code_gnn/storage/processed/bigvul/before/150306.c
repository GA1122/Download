PasswordAutofillAgent::GetSimplifiedPasswordFormFromUnownedInputElements() {
  content::RenderFrame* frame = render_frame();
  if (!frame)
    return nullptr;
  WebLocalFrame* web_frame = frame->GetWebFrame();
  if (!web_frame)
    return nullptr;
  return CreateSimplifiedPasswordFormFromUnownedInputElements(
      *web_frame, &field_data_manager_, &username_detector_cache_);
}
