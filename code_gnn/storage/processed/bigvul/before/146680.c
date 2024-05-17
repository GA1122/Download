void PageInfoBubbleView::SetPermissionInfo(
    const PermissionInfoList& permission_info_list,
    ChosenObjectInfoList chosen_object_info_list) {
  if (permissions_view_->has_children())
    return;

  GridLayout* layout = GridLayout::CreateAndInstall(permissions_view_);

  ChromeLayoutProvider* layout_provider = ChromeLayoutProvider::Get();
  const int list_item_padding =
      layout_provider->GetDistanceMetric(DISTANCE_CONTROL_LIST_VERTICAL);
  if (!permission_info_list.empty() || !chosen_object_info_list.empty()) {
    layout->AddPaddingRow(0, list_item_padding);
  } else {
    layout->AddPaddingRow(0, layout_provider->GetDistanceMetric(
                                 views::DISTANCE_UNRELATED_CONTROL_VERTICAL));
    return;
  }

  constexpr float kFixed = 0.f;
  constexpr float kStretchy = 1.f;
  int side_margin =
      layout_provider->GetInsetsMetric(views::INSETS_DIALOG).left();
  views::ColumnSet* permissions_set =
      layout->AddColumnSet(kPermissionColumnSetId);
  permissions_set->AddPaddingColumn(kFixed, side_margin);
  permissions_set->AddColumn(GridLayout::CENTER, GridLayout::CENTER, kFixed,
                             GridLayout::FIXED, kIconColumnWidth, 0);
  permissions_set->AddPaddingColumn(
      kFixed, layout_provider->GetDistanceMetric(
                  views::DISTANCE_RELATED_LABEL_HORIZONTAL));
  permissions_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER, kFixed,
                             GridLayout::USE_PREF, 0, 0);
  permissions_set->AddPaddingColumn(
      1, layout_provider->GetDistanceMetric(
             views::DISTANCE_RELATED_CONTROL_HORIZONTAL));
  permissions_set->AddColumn(GridLayout::TRAILING, GridLayout::FILL, kStretchy,
                             GridLayout::USE_PREF, 0, 0);
  permissions_set->AddPaddingColumn(kFixed, side_margin);

  constexpr int kChosenObjectSectionId = 1;
  views::ColumnSet* chosen_object_set =
      layout->AddColumnSet(kChosenObjectSectionId);
  chosen_object_set->AddPaddingColumn(kFixed, side_margin);
  chosen_object_set->AddColumn(GridLayout::FILL, GridLayout::FILL, kStretchy,
                               GridLayout::USE_PREF, 0, 0);
  chosen_object_set->AddPaddingColumn(kFixed, side_margin);

  for (const auto& permission : permission_info_list) {
    std::unique_ptr<PermissionSelectorRow> selector =
        base::MakeUnique<PermissionSelectorRow>(
            profile_,
            web_contents() ? web_contents()->GetVisibleURL()
                           : GURL::EmptyGURL(),
            permission, layout);
    selector->AddObserver(this);
    selector_rows_.push_back(std::move(selector));
  }

  if (ui::MaterialDesignController::IsSecondaryUiMaterial()) {
    const int maximum_width = ChromeLayoutProvider::Get()->GetDistanceMetric(
        views::DISTANCE_BUTTON_MAX_LINKABLE_WIDTH);
    int combobox_width = 0;
    for (const auto& selector : selector_rows_) {
      int curr_width = selector->GetComboboxWidth();
      if (maximum_width >= curr_width)
        combobox_width = std::max(combobox_width, curr_width);
    }
    for (const auto& selector : selector_rows_)
      selector->SetMinComboboxWidth(combobox_width);
  }

  for (auto& object : chosen_object_info_list) {
    layout->StartRow(
        1, kChosenObjectSectionId,
        PermissionSelectorRow::MinHeightForPermissionRow() + list_item_padding);
    auto object_view = std::make_unique<ChosenObjectRow>(std::move(object));
    object_view->AddObserver(this);
    layout->AddView(object_view.release());
  }
  layout->AddPaddingRow(kFixed, list_item_padding);

  layout->Layout(permissions_view_);
  SizeToContents();
}
