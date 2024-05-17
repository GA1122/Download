void WebsiteSettingsPopupView::ResetConnectionSection(
    views::View* section_container,
    const gfx::Image& icon,
    const base::string16& headline,
    const base::string16& text,
    views::Link* link,
    views::LabelButton* reset_decisions_button) {
  section_container->RemoveAllChildViews(true);

  views::GridLayout* layout = new views::GridLayout(section_container);
  section_container->SetLayoutManager(layout);
  views::ColumnSet* column_set = layout->AddColumnSet(0);
  column_set->AddPaddingColumn(0, kConnectionSectionPaddingLeft);
  column_set->AddColumn(views::GridLayout::LEADING,
                        views::GridLayout::LEADING,
                        0,
                        views::GridLayout::USE_PREF,
                        0,
                        0);
  column_set->AddPaddingColumn(0, kIconMarginLeft);
  column_set->AddColumn(views::GridLayout::FILL,
                        views::GridLayout::FILL,
                        1,
                        views::GridLayout::USE_PREF,
                        0,
                        0);
  column_set->AddPaddingColumn(0, kConnectionSectionPaddingRight);


  layout->AddPaddingRow(0, kConnectionSectionPaddingTop);
  layout->StartRow(1, 0);

  views::ImageView* icon_view = new views::ImageView();
  icon_view->SetImage(*icon.ToImageSkia());
  layout->AddView(icon_view, 1, 1, views::GridLayout::LEADING,
                  views::GridLayout::LEADING);

  views::View* content_pane = new views::View();
  views::GridLayout* content_layout = new views::GridLayout(content_pane);
  content_pane->SetLayoutManager(content_layout);
  views::ColumnSet* content_column_set = content_layout->AddColumnSet(0);
  content_column_set->AddColumn(views::GridLayout::LEADING,
                                views::GridLayout::LEADING,
                                1,
                                views::GridLayout::USE_PREF,
                                0,
                                0);
  if (!headline.empty()) {
    ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
    views::Label* headline_label = new views::Label(
        headline, rb.GetFontList(ui::ResourceBundle::BoldFont));
    headline_label->SetMultiLine(true);
    headline_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    headline_label->SetAllowCharacterBreak(true);
    content_layout->StartRow(1, 0);
    content_layout->AddView(headline_label);
  }

  views::Label* description_label = new views::Label(text);
  description_label->SetMultiLine(true);
  description_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  description_label->SetAllowCharacterBreak(true);
  content_layout->StartRow(1, 0);
  content_layout->AddView(description_label);

  if (link) {
    content_layout->StartRow(1, 0);
    content_layout->AddView(link);
  }

  if (reset_decisions_button) {
    content_layout->StartRow(1, 0);
    content_layout->AddView(reset_decisions_button);
  }

  layout->AddView(content_pane, 1, 1, views::GridLayout::LEADING,
                  views::GridLayout::LEADING);
  layout->AddPaddingRow(0, kConnectionSectionPaddingBottom);
}
