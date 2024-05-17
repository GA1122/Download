WebLocalFrameImpl::WebLocalFrameImpl(
    WebTreeScopeType scope,
    WebFrameClient* client,
    blink::InterfaceRegistry* interface_registry)
    : WebLocalFrameBase(scope),
      local_frame_client_(LocalFrameClientImpl::Create(this)),
      client_(client),
      autofill_client_(0),
      input_events_scale_factor_for_emulation_(1),
      interface_registry_(interface_registry),
      web_dev_tools_frontend_(0),
      input_method_controller_(*this),
      text_checker_client_(new TextCheckerClientImpl(this)),
      spell_check_panel_host_client_(nullptr),
      self_keep_alive_(this) {
  DCHECK(client_);
  g_frame_count++;
  client_->BindToFrame(this);
}
