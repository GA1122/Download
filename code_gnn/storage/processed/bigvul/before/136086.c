views::View* WebsiteSettingsPopupView::CreateSection(
    const base::string16& headline_text,
    views::View* content,
    views::Link* link) {
  views::View* container = new views::View();
  views::GridLayout* layout = new views::GridLayout(container);
  container->SetLayoutManager(layout);
  const int content_column = 0;
  views::ColumnSet* column_set = layout->AddColumnSet(content_column);
  column_set->AddPaddingColumn(0, kPermissionsSectionPaddingLeft);
  column_set->AddColumn(views::GridLayout::FILL,
                        views::GridLayout::FILL,
                        1,
                        views::GridLayout::USE_PREF,
                        0,
                        0);

  if (headline_text.length() > 0) {
    layout->AddPaddingRow(1, kPermissionsSectionPaddingTop);
    layout->StartRow(1, content_column);
    ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
    views::Label* headline = new views::Label(
        headline_text, rb.GetFontList(ui::ResourceBundle::BoldFont));
    layout->AddView(headline, 1, 1, views::GridLayout::LEADING,
                    views::GridLayout::CENTER);
  }

  layout->AddPaddingRow(1, kPermissionsSectionHeadlineMarginBottom);
  layout->StartRow(1, content_column);
  layout->AddView(content, 1, 1, views::GridLayout::LEADING,
                  views::GridLayout::CENTER);

  if (link) {
    layout->AddPaddingRow(1, 4);
    layout->StartRow(1, content_column);
    layout->AddView(link, 1, 1, views::GridLayout::LEADING,
                    views::GridLayout::CENTER);
  }

  layout->AddPaddingRow(1, kPermissionsSectionPaddingBottom);
  return container;
}
