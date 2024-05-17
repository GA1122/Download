InfoBarBackground::InfoBarBackground(InfoBarDelegate::Type infobar_type) {
  SkColor top_color;
  SkColor bottom_color;
  switch (infobar_type) {
    case InfoBarDelegate::WARNING_TYPE:
      top_color = kWarningBackgroundColorTop;
      bottom_color = kWarningBackgroundColorBottom;
      break;
    case InfoBarDelegate::PAGE_ACTION_TYPE:
      top_color = kPageActionBackgroundColorTop;
      bottom_color = kPageActionBackgroundColorBottom;
      break;
    default:
      NOTREACHED();
      break;
  }
  gradient_background_.reset(
      views::Background::CreateVerticalGradientBackground(top_color,
                                                          bottom_color));
}
