  ReadOnlyOriginView(const base::string16& page_title,
                     const GURL& origin,
                     gfx::ImageSkia icon_image_skia,
                     SkColor background_color,
                     views::ButtonListener* site_settings_listener) {
    std::unique_ptr<views::View> title_origin_container =
        std::make_unique<views::View>();
    SkColor foreground = color_utils::GetColorWithMaxContrast(background_color);
    views::GridLayout* title_origin_layout =
        title_origin_container->SetLayoutManager(
            std::make_unique<views::GridLayout>(title_origin_container.get()));

    views::ColumnSet* columns = title_origin_layout->AddColumnSet(0);
    columns->AddColumn(views::GridLayout::LEADING, views::GridLayout::FILL, 1.0,
                       views::GridLayout::USE_PREF, 0, 0);

    bool title_is_valid = !page_title.empty();
    if (title_is_valid) {
      title_origin_layout->StartRow(views::GridLayout::kFixedSize, 0);
      std::unique_ptr<views::Label> title_label =
          std::make_unique<views::Label>(page_title,
                                         views::style::CONTEXT_DIALOG_TITLE);
      title_label->set_id(static_cast<int>(DialogViewID::SHEET_TITLE));
      title_label->SetFocusBehavior(
          views::View::FocusBehavior::ACCESSIBLE_ONLY);
      title_label->SetAutoColorReadabilityEnabled(false);
      title_label->SetEnabledColor(foreground);
      title_origin_layout->AddView(title_label.release());
    }

    title_origin_layout->StartRow(views::GridLayout::kFixedSize, 0);
    views::Label* origin_label =
        new views::Label(base::UTF8ToUTF16(origin.host()));
    origin_label->SetElideBehavior(gfx::ELIDE_HEAD);
    if (!title_is_valid) {
      origin_label->set_id(static_cast<int>(DialogViewID::SHEET_TITLE));

      constexpr int kVerticalPadding = 10;
      origin_label->SetBorder(
          views::CreateEmptyBorder(kVerticalPadding, 0, kVerticalPadding, 0));
    }
    origin_label->SetAutoColorReadabilityEnabled(false);
    origin_label->SetEnabledColor(foreground);

    origin_label->SetBackgroundColor(background_color);
    title_origin_layout->AddView(origin_label);

    views::GridLayout* top_level_layout =
        SetLayoutManager(std::make_unique<views::GridLayout>(this));
    views::ColumnSet* top_level_columns = top_level_layout->AddColumnSet(0);
    top_level_columns->AddColumn(views::GridLayout::LEADING,
                                 views::GridLayout::CENTER, 1.0,
                                 views::GridLayout::USE_PREF, 0, 0);
    const bool has_icon = icon_image_skia.width() && icon_image_skia.height();
    float adjusted_width = base::checked_cast<float>(icon_image_skia.width());
    if (has_icon) {
      adjusted_width =
          adjusted_width *
          IconSizeCalculator::kPaymentAppDeviceIndependentIdealIconHeight /
          icon_image_skia.height();
      top_level_columns->AddColumn(
          views::GridLayout::LEADING, views::GridLayout::FILL,
          views::GridLayout::kFixedSize, views::GridLayout::FIXED,
          adjusted_width,
          IconSizeCalculator::kPaymentAppDeviceIndependentIdealIconHeight);
      top_level_columns->AddPaddingColumn(views::GridLayout::kFixedSize, 8);
    }

    top_level_layout->StartRow(views::GridLayout::kFixedSize, 0);
    top_level_layout->AddView(title_origin_container.release());
    if (has_icon) {
      std::unique_ptr<views::ImageView> instrument_icon_view =
          CreateInstrumentIconView( 0, icon_image_skia,
                                    page_title);
      instrument_icon_view->SetImageSize(gfx::Size(
          adjusted_width,
          IconSizeCalculator::kPaymentAppDeviceIndependentIdealIconHeight));
      top_level_layout->AddView(instrument_icon_view.release());
    }
  }
