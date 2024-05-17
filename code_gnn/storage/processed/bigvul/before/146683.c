void PageInfoBubbleView::StyledLabelLinkClicked(views::StyledLabel* label,
                                                const gfx::Range& range,
                                                int event_flags) {
  switch (label->id()) {
    case PageInfoBubbleView::VIEW_ID_PAGE_INFO_LABEL_SECURITY_DETAILS:
      web_contents()->OpenURL(content::OpenURLParams(
          GURL(chrome::kPageInfoHelpCenterURL), content::Referrer(),
          WindowOpenDisposition::NEW_FOREGROUND_TAB, ui::PAGE_TRANSITION_LINK,
          false));
      presenter_->RecordPageInfoAction(
          PageInfo::PAGE_INFO_CONNECTION_HELP_OPENED);
      break;
    case PageInfoBubbleView::
        VIEW_ID_PAGE_INFO_LABEL_RESET_CERTIFICATE_DECISIONS:
      presenter_->OnRevokeSSLErrorBypassButtonPressed();
      GetWidget()->Close();
      break;
    default:
      NOTREACHED();
  }
}
