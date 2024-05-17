bool IsSureError(const autofill::ValidityMessage& message) {
  return message.sure && !message.text.empty();
}
