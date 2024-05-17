void HTMLInputElement::ResetListAttributeTargetObserver() {
  const AtomicString& value = FastGetAttribute(listAttr);
  if (!value.IsNull() && isConnected()) {
    SetListAttributeTargetObserver(
        ListAttributeTargetObserver::Create(value, this));
  } else {
    SetListAttributeTargetObserver(nullptr);
  }
}
