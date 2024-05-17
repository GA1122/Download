base::string16 SubBrowserName(const char* fmt) {
  return base::UTF8ToUTF16(base::StringPrintf(
      fmt, l10n_util::GetStringUTF8(IDS_PRODUCT_NAME).c_str()));
}
