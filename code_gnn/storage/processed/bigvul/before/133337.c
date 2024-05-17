  explicit TitleView(PaletteTray* palette_tray) : palette_tray_(palette_tray) {
    auto& rb = ui::ResourceBundle::GetSharedInstance();

    auto* box_layout =
        new views::BoxLayout(views::BoxLayout::kHorizontal, 0, 0, 0);
    SetLayoutManager(box_layout);
    SetBorder(views::Border::CreateEmptyBorder(
        0, ash::kTrayPopupPaddingHorizontal, 0, 0));

    views::Label* text_label =
        new views::Label(l10n_util::GetStringUTF16(IDS_ASH_PALETTE_TITLE));
    text_label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
    text_label->SetFontList(rb.GetFontList(ui::ResourceBundle::BoldFont));
    AddChildView(text_label);
    box_layout->SetFlexForView(text_label, 1);

    help_button_ = new ash::TrayPopupHeaderButton(this, IDR_AURA_UBER_TRAY_HELP,
                                                  IDS_ASH_STATUS_TRAY_HELP);
    help_button_->SetTooltipText(
        l10n_util::GetStringUTF16(IDS_ASH_STATUS_TRAY_SHUTDOWN));
    AddChildView(help_button_);

    AddChildView(CreateSeparator(views::Separator::VERTICAL));

    settings_button_ = new ash::TrayPopupHeaderButton(
        this, IDR_AURA_UBER_TRAY_SETTINGS, IDS_ASH_STATUS_TRAY_SETTINGS);
    settings_button_->SetTooltipText(
        l10n_util::GetStringUTF16(IDS_ASH_STATUS_TRAY_SETTINGS));
    AddChildView(settings_button_);
  }
