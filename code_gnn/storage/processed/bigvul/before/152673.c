static bool shouldAutofocusOnAttach(const HTMLFormControlElement* element) {
  if (!element->isAutofocusable())
    return false;
  if (element->document().isSandboxed(SandboxAutomaticFeatures)) {
    element->document().addConsoleMessage(ConsoleMessage::create(
        SecurityMessageSource, ErrorMessageLevel,
        "Blocked autofocusing on a form control because the form's frame is "
        "sandboxed and the 'allow-scripts' permission is not set."));
    return false;
  }

  return true;
}
