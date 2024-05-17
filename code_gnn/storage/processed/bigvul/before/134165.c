void InputMethodTSF::OnFocus() {

  tsf_event_router_->SetManager(
      ui::TSFBridge::GetInstance()->GetThreadManager());
}
