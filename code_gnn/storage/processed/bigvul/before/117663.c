SessionCrashedInfoBarDelegate::SessionCrashedInfoBarDelegate(
    Profile* profile,
    InfoBarTabHelper* infobar_helper)
    : ConfirmInfoBarDelegate(infobar_helper),
      profile_(profile) {
}
