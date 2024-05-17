bool ExtensionInstallDialogView::AddPermissions(
    views::GridLayout* layout,
    ui::ResourceBundle& rb,
    int column_set_id,
    int left_column_width,
    ExtensionInstallPrompt::PermissionsType perm_type) {
  if (prompt_->GetPermissionCount(perm_type) == 0)
    return false;

  const int vertical_padding = LayoutDelegate::Get()->GetMetric(
      LayoutDelegate::Metric::RELATED_CONTROL_VERTICAL_SPACING);
  layout->AddPaddingRow(0, vertical_padding);

  layout->StartRow(0, column_set_id);
  views::Label* permissions_header =
      new views::Label(prompt_->GetPermissionsHeading(perm_type));
  permissions_header->SetMultiLine(true);
  permissions_header->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  permissions_header->SizeToFit(left_column_width);
  layout->AddView(permissions_header);

  for (size_t i = 0; i < prompt_->GetPermissionCount(perm_type); ++i) {
    layout->AddPaddingRow(0, vertical_padding);
    layout->StartRow(0, column_set_id);
    views::Label* permission_label =
        new views::Label(prompt_->GetPermission(i, perm_type));

    permission_label->SetMultiLine(true);
    permission_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    permission_label->SizeToFit(left_column_width - kBulletWidth);
    layout->AddView(new BulletedView(permission_label));

    if (!prompt_->GetPermissionsDetails(i, perm_type).empty()) {
      layout->StartRow(0, column_set_id);
      PermissionDetails details;
      details.push_back(PrepareForDisplay(
          prompt_->GetPermissionsDetails(i, perm_type), false));
      ExpandableContainerView* details_container =
          new ExpandableContainerView(details, left_column_width, true);
      layout->AddView(details_container);
    }
  }
  return true;
}
