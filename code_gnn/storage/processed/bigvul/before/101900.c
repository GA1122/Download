std::wstring WindowCaptionFromPageTitle(std::wstring page_title) {
#if defined(OS_MACOSX) || defined(OS_CHROMEOS)
  if (page_title.empty()) {
    return UTF16ToWideHack(
        l10n_util::GetStringUTF16(IDS_BROWSER_WINDOW_MAC_TAB_UNTITLED));
  }
  return page_title;
#else
  if (page_title.empty())
    return UTF16ToWideHack(l10n_util::GetStringUTF16(IDS_PRODUCT_NAME));

  return UTF16ToWideHack(
      l10n_util::GetStringFUTF16(IDS_BROWSER_WINDOW_TITLE_FORMAT,
                                 WideToUTF16Hack(page_title)));
#endif
}
