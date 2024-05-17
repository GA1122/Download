InternalPageInfoPopupView::InternalPageInfoPopupView(
    views::View* anchor_view,
    gfx::NativeView parent_window)
    : BubbleDelegateView(anchor_view, views::BubbleBorder::TOP_LEFT) {
  set_parent_window(parent_window);

  set_anchor_view_insets(gfx::Insets(kLocationIconVerticalMargin, 0,
                                     kLocationIconVerticalMargin, 0));

  const int kSpacing = 4;
  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kHorizontal, kSpacing,
                                        kSpacing, kSpacing));
  views::ImageView* icon_view = new views::ImageView();
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  icon_view->SetImage(rb.GetImageSkiaNamed(IDR_PRODUCT_LOGO_26));
  AddChildView(icon_view);

  views::Label* label =
      new views::Label(l10n_util::GetStringUTF16(IDS_PAGE_INFO_INTERNAL_PAGE));
  label->SetMultiLine(true);
  label->SetAllowCharacterBreak(true);
  label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  AddChildView(label);

  views::BubbleDelegateView::CreateBubble(this);
}
