 std::string DistillerNativeJavaScript::DistillerEcho(
     const std::string& message) {
   EnsureServiceConnected();
  distiller_js_service_->HandleDistillerEchoCall(message);

  return message;
}
