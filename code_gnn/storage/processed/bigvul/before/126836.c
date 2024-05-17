void BookmarkExtensionBackground::Paint(gfx::Canvas* canvas,
                                        views::View* view) const {
  ui::ThemeProvider* tp = host_view_->GetThemeProvider();

  const chrome::search::Mode& search_mode =
      browser_view_->browser()->search_model()->mode();
  if (search_mode.is_ntp()) {
    BookmarkModel* bookmark_model =
        BookmarkModelFactory::GetForProfile(browser_->profile());
    if (bookmark_model && bookmark_model->HasBookmarks()) {
      if (tp->HasCustomImage(IDR_THEME_NTP_BACKGROUND)) {
        const U8CPU kBackgroundOpacity = 204;   
        SkColor color = tp->GetColor(ThemeService::COLOR_NTP_BACKGROUND);
        if (gfx::IsInvertedColorScheme())
          color = color_utils::InvertColor(color);
        if (SkColorGetA(color) > kBackgroundOpacity)
          color = SkColorSetA(color, kBackgroundOpacity);
        canvas->DrawColor(color);
        DetachableToolbarView::PaintHorizontalBorder(canvas, host_view_);
      } else {
        const SkColor kBorderColor = SkColorSetARGB(25, 0, 0, 0);   
        DetachableToolbarView::PaintHorizontalBorderWithColor(
            canvas, host_view_, kBorderColor);
      }
    }
    return;
  }

  int toolbar_overlap = host_view_->GetToolbarOverlap();
  if (toolbar_overlap)
    toolbar_overlap += views::NonClientFrameView::kClientEdgeThickness;
  if (host_view_->IsDetached()) {
    int height = 0;
    WebContents* contents = chrome::GetActiveWebContents(browser_);
    if (contents && contents->GetView())
      height = contents->GetView()->GetContainerSize().height();
    NtpBackgroundUtil::PaintBackgroundDetachedMode(
        tp, canvas,
        gfx::Rect(0, toolbar_overlap, host_view_->width(),
                  host_view_->height() - toolbar_overlap),
        height);

    double current_state = 1 - host_view_->GetAnimationValue();
    double h_padding =
        static_cast<double>(BookmarkBarView::kNewtabHorizontalPadding) *
        current_state;
    double v_padding =
        static_cast<double>(BookmarkBarView::kNewtabVerticalPadding) *
        current_state;

    SkRect rect;
    double roundness = 0;
    DetachableToolbarView::CalculateContentArea(current_state, h_padding,
        v_padding, &rect, &roundness, host_view_);
    DetachableToolbarView::PaintContentAreaBackground(canvas, tp, rect,
                                                      roundness);
    DetachableToolbarView::PaintContentAreaBorder(canvas, tp, rect, roundness);
    if (!toolbar_overlap)
      DetachableToolbarView::PaintHorizontalBorder(canvas, host_view_);
  } else {
    DetachableToolbarView::PaintBackgroundAttachedMode(canvas, host_view_,
        browser_view_->OffsetPointForToolbarBackgroundImage(
            gfx::Point(host_view_->GetMirroredX(), host_view_->y())));
    if (host_view_->height() >= toolbar_overlap)
      DetachableToolbarView::PaintHorizontalBorder(canvas, host_view_);
  }
}
