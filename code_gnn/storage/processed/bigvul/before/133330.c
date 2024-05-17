bool PaletteTray::OpenBubble() {
  views::TrayBubbleView::InitParams init_params(
      views::TrayBubbleView::ANCHOR_TYPE_TRAY, GetAnchorAlignment(),
      kPaletteWidth, kPaletteWidth);
  init_params.first_item_has_no_margin = true;
  init_params.can_activate = true;
  init_params.close_on_deactivate = true;

  DCHECK(tray_container());

  views::TrayBubbleView* bubble_view =
      views::TrayBubbleView::Create(tray_container(), this, &init_params);
  bubble_view->SetArrowPaintType(views::BubbleBorder::PAINT_NONE);
  bubble_view->UseCompactMargins();
  bubble_view->set_margins(gfx::Insets(bubble_view->margins().top(), 0, 0, 0));

  bubble_view->AddChildView(new TitleView(this));
  bubble_view->AddChildView(CreateSeparator(views::Separator::HORIZONTAL));
  AddToolsToView(bubble_view);

  bubble_.reset(new ash::TrayBubbleWrapper(this, bubble_view));

  SetDrawBackgroundAsActive(true);

  return true;
}
