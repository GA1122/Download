base::string16 PrepareForDisplay(const base::string16& message,
                                 bool bullet_point) {
  return bullet_point ? l10n_util::GetStringFUTF16(
      IDS_EXTENSION_PERMISSION_LINE,
      message) : message;
}
