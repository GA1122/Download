HTMLFormControlsCollection* HTMLFormElement::elements() {
  return EnsureCachedCollection<HTMLFormControlsCollection>(kFormControls);
}
