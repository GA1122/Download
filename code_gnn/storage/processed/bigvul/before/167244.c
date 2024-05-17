bool ConfirmInfoBarDelegate::LinkClicked(WindowOpenDisposition disposition) {
  infobar()->owner()->OpenURL(GetLinkURL(), disposition);
  return false;
}
