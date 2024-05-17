void AutofillPopupItemView::CreateContent() {
  AutofillPopupController* controller = popup_view_->controller();

  auto* layout_manager = SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::kHorizontal, gfx::Insets(0, GetHorizontalMargin())));

  layout_manager->set_cross_axis_alignment(
      views::BoxLayout::CrossAxisAlignment::CROSS_AXIS_ALIGNMENT_STRETCH);

  const gfx::ImageSkia icon =
      controller->layout_model().GetIconImage(line_number_);

  if (!icon.isNull() &&
      (GetLayoutType() == PopupItemLayoutType::kLeadingIcon ||
       GetLayoutType() == PopupItemLayoutType::kTwoLinesLeadingIcon)) {
    AddIcon(icon);
    AddSpacerWithSize(views::MenuConfig::instance().item_horizontal_padding,
                       false, layout_manager);
  }

  views::View* lower_value_label = CreateSubtextLabel();
  views::View* value_label = CreateValueLabel();

  const int kStandardRowHeight =
      views::MenuConfig::instance().touchable_menu_height + extra_height_;
  if (!lower_value_label) {
    layout_manager->set_minimum_cross_axis_size(kStandardRowHeight);
    AddChildView(value_label);
  } else {
    layout_manager->set_minimum_cross_axis_size(
        kStandardRowHeight + kAutofillPopupAdditionalDoubleRowHeight);
    views::View* values_container = new views::View();
    auto* vertical_layout =
        values_container->SetLayoutManager(std::make_unique<views::BoxLayout>(
            views::BoxLayout::kVertical, gfx::Insets(),
            kAdjacentLabelsVerticalSpacing));
    vertical_layout->set_main_axis_alignment(
        views::BoxLayout::MAIN_AXIS_ALIGNMENT_CENTER);
    vertical_layout->set_cross_axis_alignment(
        views::BoxLayout::CROSS_AXIS_ALIGNMENT_START);
    values_container->AddChildView(value_label);
    values_container->AddChildView(lower_value_label);
    AddChildView(values_container);
  }

  AddSpacerWithSize(AutofillPopupBaseView::kValueLabelPadding,
                     true, layout_manager);

  views::View* description_label = CreateDescriptionLabel();
  if (description_label)
    AddChildView(description_label);

  if (!icon.isNull() && GetLayoutType() == PopupItemLayoutType::kTrailingIcon) {
    AddSpacerWithSize(views::MenuConfig::instance().item_horizontal_padding,
                       false, layout_manager);
    AddIcon(icon);
  }
}
