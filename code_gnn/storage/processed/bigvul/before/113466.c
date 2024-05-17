void InputMethodIBus::PendingCreateICRequestImpl::StoreOrAbandonInputContext(
    IBusInputContext* ic) {
  if (!ic)
    return;

  if (input_method_) {
    input_method_->SetContext(ic);
  } else {
    ibus_client_->DestroyProxy(ic);
    g_object_unref(ic);
  }
}
