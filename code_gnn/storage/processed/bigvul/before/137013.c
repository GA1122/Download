void HTMLInputElement::setChecked(bool now_checked,
                                  TextFieldEventBehavior event_behavior) {
  dirty_checkedness_ = true;
  if (checked() == now_checked)
    return;

  is_checked_ = now_checked;

  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    scope->UpdateCheckedState(this);
  if (LayoutObject* o = GetLayoutObject())
    o->InvalidateIfControlStateChanged(kCheckedControlState);
  SetNeedsValidityCheck();

  if (GetLayoutObject()) {
    if (AXObjectCache* cache =
            GetLayoutObject()->GetDocument().ExistingAXObjectCache())
      cache->CheckedStateChanged(this);
  }

  if (event_behavior == kDispatchInputAndChangeEvent && isConnected() &&
      input_type_->ShouldSendChangeEventAfterCheckedChanged()) {
    DispatchInputEvent();
  }

  PseudoStateChanged(CSSSelector::kPseudoChecked);
}
