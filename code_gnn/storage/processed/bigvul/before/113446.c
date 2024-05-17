void InputMethodIBus::OnDestroy(IBusInputContext* context) {
  DCHECK_EQ(context_, context);
  g_object_unref(context_);
  context_ = NULL;
  context_focused_ = false;

  ConfirmCompositionText();

  OnInputMethodChanged();
}
