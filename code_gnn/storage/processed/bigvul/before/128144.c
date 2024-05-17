void CastSelectDefaultView::UpdateLabelCallback(
    const CastConfigDelegate::ReceiversAndActivites& receivers_activities) {
  const base::string16 label =
      ui::ResourceBundle::GetSharedInstance().GetLocalizedString(
          receivers_activities.empty() ? IDS_ASH_STATUS_TRAY_CAST_NO_DEVICE
                                       : IDS_ASH_STATUS_TRAY_CAST_DESKTOP);
  SetLabel(label);
  SetAccessibleName(label);
  SetVisible(true);
}
