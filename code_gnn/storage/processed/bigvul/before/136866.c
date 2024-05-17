ListAttributeTargetObserver* ListAttributeTargetObserver::Create(
    const AtomicString& id,
    HTMLInputElement* element) {
  return new ListAttributeTargetObserver(id, element);
}
