WebLocalFrameImpl::WebLocalFrameImpl(
    WebTreeScopeType scope,
    WebLocalFrameClient* client,
    blink::InterfaceRegistry* interface_registry)
    : WebLocalFrame(scope),
      client_(client),
      local_frame_client_(LocalFrameClientImpl::Create(this)),
      autofill_client_(nullptr),
      find_in_page_(FindInPage::Create(*this, interface_registry)),
      input_events_scale_factor_for_emulation_(1),
      interface_registry_(interface_registry),
      input_method_controller_(*this),
      spell_check_panel_host_client_(nullptr),
      self_keep_alive_(this) {
  DCHECK(client_);
  g_frame_count++;
  client_->BindToFrame(this);
}
