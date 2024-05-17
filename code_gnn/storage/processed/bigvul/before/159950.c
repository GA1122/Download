std::string WebUIExtension::GetVariableValue(const std::string& name) {
  blink::WebLocalFrame* frame;
  RenderFrame* render_frame;
  if (!ShouldRespondToRequest(&frame, &render_frame))
    return std::string();

  return WebUIExtensionData::Get(render_frame->GetRenderView())->GetValue(name);
}
