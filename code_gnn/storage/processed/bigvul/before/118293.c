AutofillDialogViews::SectionContainer::SectionContainer(
    const base::string16& label,
    views::View* controls,
    views::Button* proxy_button)
    : proxy_button_(proxy_button),
      forward_mouse_events_(false) {
  set_notify_enter_exit_on_child(true);

  SetBorder(views::Border::CreateEmptyBorder(kDetailSectionVerticalPadding,
                                             kDialogEdgePadding,
                                             kDetailSectionVerticalPadding,
                                             kDialogEdgePadding));

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  views::Label* label_view = new views::Label(
      label, rb.GetFontList(ui::ResourceBundle::BoldFont));
  label_view->SetHorizontalAlignment(gfx::ALIGN_LEFT);

  views::View* label_bar = new views::View();
  views::GridLayout* label_bar_layout = new views::GridLayout(label_bar);
  label_bar->SetLayoutManager(label_bar_layout);
  const int kColumnSetId = 0;
  views::ColumnSet* columns = label_bar_layout->AddColumnSet(kColumnSetId);
  columns->AddColumn(
      views::GridLayout::LEADING,
      views::GridLayout::LEADING,
      0,
      views::GridLayout::FIXED,
      kSectionContainerWidth - proxy_button->GetPreferredSize().width(),
      0);
  columns->AddColumn(views::GridLayout::LEADING,
                     views::GridLayout::LEADING,
                     0,
                     views::GridLayout::USE_PREF,
                     0,
                     0);
  label_bar_layout->StartRow(0, kColumnSetId);
  label_bar_layout->AddView(label_view);
  label_bar_layout->AddView(proxy_button);

  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 0));
  AddChildView(label_bar);
  AddChildView(controls);

  SetEventTargeter(
      scoped_ptr<views::ViewTargeter>(new views::ViewTargeter(this)));
}
