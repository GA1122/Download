void HomePageUndoBubble::Init() {
  views::GridLayout* layout = new views::GridLayout(this);
  SetLayoutManager(layout);

  views::ColumnSet* cs = layout->AddColumnSet(0);
  cs = layout->AddColumnSet(1);
  cs->AddColumn(views::GridLayout::LEADING, views::GridLayout::BASELINE, 0,
                views::GridLayout::USE_PREF, 0, 0);
  cs->AddPaddingColumn(0, views::kRelatedControlHorizontalSpacing);
  cs->AddColumn(views::GridLayout::CENTER, views::GridLayout::BASELINE, 0,
                views::GridLayout::USE_PREF, 0, 0);

  views::Label* message_label = new views::Label(
      l10n_util::GetStringUTF16(IDS_TOOLBAR_INFORM_SET_HOME_PAGE));
  message_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  layout->StartRow(0, 1);
  layout->AddView(message_label);

  views::Link* undo_link = new views::Link(
      l10n_util::GetStringUTF16(IDS_ONE_CLICK_BUBBLE_UNDO));
  undo_link->set_listener(this);
  layout->AddView(undo_link);
}
