void PageInfoBubbleView::ButtonPressed(views::Button* button,
                                       const ui::Event& event) {
  switch (button->id()) {
    case PageInfoBubbleView::VIEW_ID_PAGE_INFO_BUTTON_CLOSE:
      GetWidget()->Close();
      break;
    case PageInfoBubbleView::VIEW_ID_PAGE_INFO_BUTTON_CHANGE_PASSWORD:
      presenter_->OnChangePasswordButtonPressed(web_contents());
      break;
    case PageInfoBubbleView::VIEW_ID_PAGE_INFO_BUTTON_WHITELIST_PASSWORD_REUSE:
      GetWidget()->Close();
      presenter_->OnWhitelistPasswordReuseButtonPressed(web_contents());
      break;
    default:
      NOTREACHED();
  }
}
