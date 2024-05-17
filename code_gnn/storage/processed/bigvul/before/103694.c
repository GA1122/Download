void DevToolsAgent::OnAttach(
    const DevToolsRuntimeProperties& runtime_properties) {
  WebDevToolsAgent* web_agent = GetWebAgent();
  if (web_agent) {
    web_agent->attach();
    for (DevToolsRuntimeProperties::const_iterator it =
             runtime_properties.begin();
         it != runtime_properties.end(); ++it) {
      web_agent->setRuntimeProperty(WebString::fromUTF8(it->first),
                                    WebString::fromUTF8(it->second));
    }
  }
}
