CastSelectDefaultView::CastSelectDefaultView(
    SystemTrayItem* owner,
    CastConfigDelegate* cast_config_delegate,
    bool show_more)
    : TrayItemMore(owner, show_more),
      cast_config_delegate_(cast_config_delegate) {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  SetImage(rb.GetImageNamed(IDR_AURA_UBER_TRAY_CAST).ToImageSkia());

  SetLabel(rb.GetLocalizedString(IDS_ASH_STATUS_TRAY_CAST_NO_DEVICE));
  UpdateLabel();
}
