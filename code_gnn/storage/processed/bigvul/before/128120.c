views::View* CastDetailedView::AddToReceiverList(
    const CastConfigDelegate::ReceiverAndActivity& receiverActivity) {
  HoverHighlightView* container = new HoverHighlightView(this);

  const gfx::ImageSkia* image =
      ui::ResourceBundle::GetSharedInstance()
          .GetImageNamed(IDR_AURA_UBER_TRAY_CAST_DEVICE_ICON)
          .ToImageSkia();
  const base::string16& name = receiverActivity.receiver.name;
  container->AddIndentedIconAndLabel(*image, name, false);

  scroll_content()->AddChildView(container);
  return container;
}
