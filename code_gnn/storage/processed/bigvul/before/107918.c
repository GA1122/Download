InfoBarContainer::InfoBarContainer(Delegate* delegate)
    : delegate_(delegate),
      tab_contents_(NULL) {
  SetID(VIEW_ID_INFO_BAR_CONTAINER);
  SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_INFOBAR_CONTAINER));
}
