StringOrdinal ExtensionPrefs::CreateFirstAppLaunchOrdinal(
    const StringOrdinal& page_ordinal) const {
  const StringOrdinal& min_ordinal =
      GetMinOrMaxAppLaunchOrdinalsOnPage(page_ordinal,
                                         ExtensionPrefs::MIN_ORDINAL);

  if (min_ordinal.IsValid())
    return min_ordinal.CreateBefore();
  else
    return StringOrdinal::CreateInitialOrdinal();
}
