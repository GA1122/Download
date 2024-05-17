ExpandableContainerView::ExpandableContainerView(
    ExtensionInstallDialogView* owner,
    const base::string16& description,
    const PermissionDetails& details,
    int horizontal_space,
    bool parent_bulleted)
    : owner_(owner),
      details_view_(NULL),
      slide_animation_(this),
      more_details_(NULL),
      arrow_toggle_(NULL),
      expanded_(false) {
  views::GridLayout* layout = new views::GridLayout(this);
  SetLayoutManager(layout);
  int column_set_id = 0;
  views::ColumnSet* column_set = layout->AddColumnSet(column_set_id);
  column_set->AddColumn(views::GridLayout::LEADING,
                        views::GridLayout::LEADING,
                        0,
                        views::GridLayout::USE_PREF,
                        0,
                        0);
  if (!description.empty()) {
    layout->StartRow(0, column_set_id);

    views::Label* description_label = new views::Label(description);
    description_label->SetMultiLine(true);
    description_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    description_label->SizeToFit(horizontal_space);
    layout->AddView(new BulletedView(description_label));
  }

  if (details.empty())
    return;

  details_view_ = new DetailsView(horizontal_space, parent_bulleted);

  layout->StartRow(0, column_set_id);
  layout->AddView(details_view_);

  for (size_t i = 0; i < details.size(); ++i)
    details_view_->AddDetail(details[i]);

  views::Link* link = new views::Link(
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_HIDE_DETAILS));
  int link_col_width = link->GetPreferredSize().width();
  link->SetText(l10n_util::GetStringUTF16(IDS_EXTENSIONS_SHOW_DETAILS));
  link_col_width = std::max(link_col_width, link->GetPreferredSize().width());

  column_set = layout->AddColumnSet(++column_set_id);
  column_set->AddPaddingColumn(
      0, views::kRelatedControlHorizontalSpacing * (parent_bulleted ? 2 : 1));
  column_set->AddColumn(views::GridLayout::LEADING,
                        views::GridLayout::LEADING,
                        0,
                        views::GridLayout::FIXED,
                        link_col_width,
                        link_col_width);
  column_set->AddColumn(views::GridLayout::LEADING,
                        views::GridLayout::TRAILING,
                        0,
                        views::GridLayout::USE_PREF,
                        0,
                        0);

  layout->StartRow(0, column_set_id);
  more_details_ = link;
  more_details_->set_listener(this);
  more_details_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  layout->AddView(more_details_);

  arrow_toggle_ = new views::ImageButton(this);
  UpdateArrowToggle(false);
  layout->AddView(arrow_toggle_);
}
