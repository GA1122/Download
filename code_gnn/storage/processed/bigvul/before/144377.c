base::string16 ExtensionInstallPrompt::Prompt::GetRetainedDevicesHeading()
    const {
  return l10n_util::GetPluralStringFUTF16(
      IDS_EXTENSION_PROMPT_RETAINED_DEVICES, GetRetainedDeviceCount());
}
