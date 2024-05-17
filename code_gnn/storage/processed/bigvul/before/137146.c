void DisableSecureTextInput() {
  if (!IsSecureEventInputEnabled())
    return;
  DisableSecureEventInput();
}
