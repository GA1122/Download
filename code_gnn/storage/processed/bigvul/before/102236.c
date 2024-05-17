StringOrdinal ExtensionPrefs::CreateNextAppLaunchOrdinal(
    const StringOrdinal& page_ordinal) const {
  const StringOrdinal& max_ordinal =
      GetMinOrMaxAppLaunchOrdinalsOnPage(page_ordinal,
                                          ExtensionPrefs::MAX_ORDINAL);

  if (max_ordinal.IsValid())
    return max_ordinal.CreateAfter();
  else
    return StringOrdinal::CreateInitialOrdinal();
}
