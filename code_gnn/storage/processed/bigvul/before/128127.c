CastTrayView::CastTrayView(SystemTrayItem* tray_item)
    : TrayItemView(tray_item), weak_ptr_factory_(this) {
  CreateImageView();

  image_view()->SetImage(ui::ResourceBundle::GetSharedInstance()
                             .GetImageNamed(IDR_AURA_UBER_TRAY_CAST_STATUS)
                             .ToImageSkia());
}
