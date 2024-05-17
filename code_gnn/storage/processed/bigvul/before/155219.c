HTMLFormControlsCollection* HTMLFormElement::elements() {
  return ensureCachedCollection<HTMLFormControlsCollection>(FormControls);
}
