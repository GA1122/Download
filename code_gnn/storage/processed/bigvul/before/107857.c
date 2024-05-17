void BlockedPlugin::BindWebFrame(WebFrame* frame) {
  BindToJavascript(frame, "plugin");
  BindMethod("load", &BlockedPlugin::Load);
  BindMethod("hide", &BlockedPlugin::Hide);
}
