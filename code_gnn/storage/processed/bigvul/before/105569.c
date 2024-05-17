Error* Automation::CheckAdvancedInteractionsSupported() {
  const char* message =
      "Advanced user interactions are not supported for this version of Chrome";
  return CheckVersion(750, 0, message);
}
