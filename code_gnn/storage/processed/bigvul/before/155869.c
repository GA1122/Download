  static views::View* AddPaddedTitleCard(views::View* view,
                                         TitleCard* title_card,
                                         int width) {
    views::View* titled_view = new views::View();
    views::GridLayout* layout = titled_view->SetLayoutManager(
        std::make_unique<views::GridLayout>(titled_view));

    ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();
    const gfx::Insets dialog_insets =
        provider->GetInsetsMetric(views::INSETS_DIALOG);
    views::ColumnSet* columns = layout->AddColumnSet(0);
    columns->AddPaddingColumn(1.0, dialog_insets.left());
    int available_width = width - dialog_insets.width();
    columns->AddColumn(views::GridLayout::FILL, views::GridLayout::FILL,
                       views::GridLayout::kFixedSize, views::GridLayout::FIXED,
                       available_width, available_width);
    columns->AddPaddingColumn(1.0, dialog_insets.right());
    layout->AddColumnSet(1)->AddColumn(
        views::GridLayout::FILL, views::GridLayout::FILL,
        views::GridLayout::kFixedSize, views::GridLayout::FIXED, width, width);

    layout->StartRowWithPadding(1.0, 0, views::GridLayout::kFixedSize,
                                kVerticalSpacing);
    layout->AddView(title_card);
    layout->StartRowWithPadding(1.0, 1.0, views::GridLayout::kFixedSize,
                                kVerticalSpacing);
    layout->AddView(new views::Separator());

    layout->StartRow(1.0, 1.0);
    layout->AddView(view);

    return titled_view;
  }
