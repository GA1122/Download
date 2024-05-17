 void RenderViewImpl::OnSetWebUIProperty(const std::string& name,
                                        const std::string& value) {
  if (enabled_bindings_ & content::BINDINGS_POLICY_WEB_UI)
    GetWebUIBindings()->SetProperty(name, value);
  else
    NOTREACHED() << "WebUI bindings not enabled.";
}
