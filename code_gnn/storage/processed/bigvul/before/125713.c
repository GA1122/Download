void RenderViewHostImpl::SetWebUIProperty(const std::string& name,
                                          const std::string& value) {
  if (enabled_bindings_ & BINDINGS_POLICY_WEB_UI)
    Send(new ViewMsg_SetWebUIProperty(GetRoutingID(), name, value));
  else
    NOTREACHED() << "WebUI bindings not enabled.";
}
