views::View* ProfileChooserView::CreateOptionsView(bool display_lock,
                                                   AvatarMenu* avatar_menu) {
  ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();
  const int content_list_vert_spacing =
      provider->GetDistanceMetric(DISTANCE_CONTENT_LIST_VERTICAL_MULTI);

  views::View* view = new views::View();
  views::GridLayout* layout = CreateSingleColumnLayout(view, menu_width_);

  const bool is_guest = browser_->profile()->IsGuestSession();
  layout->StartRowWithPadding(1.0, 0, views::GridLayout::kFixedSize,
                              content_list_vert_spacing);
  std::vector<size_t> ordered_item_indices;
  for (size_t i = 0; i < avatar_menu->GetNumberOfItems(); ++i) {
    if (avatar_menu->GetItemAt(i).active)
      ordered_item_indices.insert(ordered_item_indices.begin(), i);
    else
      ordered_item_indices.push_back(i);
  }
  for (size_t i : ordered_item_indices) {
    const AvatarMenu::Item& item = avatar_menu->GetItemAt(i);
    if (!item.active) {
      gfx::Image image = profiles::GetSizedAvatarIcon(
          item.icon, true, kIconSize, kIconSize, profiles::SHAPE_CIRCLE);
      views::LabelButton* button =
          new HoverButton(this, *image.ToImageSkia(),
                          profiles::GetProfileSwitcherTextForItem(item));
      open_other_profile_indexes_map_[button] = i;

      if (!first_profile_button_)
        first_profile_button_ = button;
      layout->StartRow(1.0, 0);
      layout->AddView(button);
    }
  }

  UMA_HISTOGRAM_BOOLEAN("ProfileChooser.HasProfilesShown",
                        first_profile_button_);

  if (!is_guest && !browser_->profile()->IsSupervised()) {
    PrefService* service = g_browser_process->local_state();
    DCHECK(service);
    if (service->GetBoolean(prefs::kBrowserGuestModeEnabled)) {
      guest_profile_button_ = new HoverButton(
          this,
          gfx::CreateVectorIcon(kUserMenuGuestIcon, kIconSize,
                                gfx::kChromeIconGrey),
          l10n_util::GetStringUTF16(IDS_PROFILES_OPEN_GUEST_PROFILE_BUTTON));
      layout->StartRow(1.0, 0);
      layout->AddView(guest_profile_button_);
    }
  }

  base::string16 text = l10n_util::GetStringUTF16(
      is_guest ? IDS_PROFILES_EXIT_GUEST : IDS_PROFILES_MANAGE_USERS_BUTTON);
  const gfx::VectorIcon& settings_icon =
      is_guest ? kCloseAllIcon : kSettingsIcon;
  users_button_ = new HoverButton(
      this,
      gfx::CreateVectorIcon(settings_icon, kIconSize, gfx::kChromeIconGrey),
      text);

  layout->StartRow(1.0, 0);
  layout->AddView(users_button_);

  if (display_lock) {
    lock_button_ = new HoverButton(
        this,
        gfx::CreateVectorIcon(vector_icons::kLockIcon, kIconSize,
                              gfx::kChromeIconGrey),
        l10n_util::GetStringUTF16(IDS_PROFILES_PROFILE_SIGNOUT_BUTTON));
    layout->StartRow(1.0, 0);
    layout->AddView(lock_button_);
  } else if (!is_guest) {
    AvatarMenu::Item active_avatar_item =
        avatar_menu->GetItemAt(ordered_item_indices[0]);
    close_all_windows_button_ = new HoverButton(
        this,
        gfx::CreateVectorIcon(kCloseAllIcon, kIconSize, gfx::kChromeIconGrey),
        avatar_menu->GetNumberOfItems() >= 2
            ? l10n_util::GetStringFUTF16(IDS_PROFILES_EXIT_PROFILE_BUTTON,
                                         active_avatar_item.name)
            : l10n_util::GetStringUTF16(IDS_PROFILES_CLOSE_ALL_WINDOWS_BUTTON));
    layout->StartRow(1.0, 0);
    layout->AddView(close_all_windows_button_);
  }

  layout->AddPaddingRow(views::GridLayout::kFixedSize,
                        content_list_vert_spacing);
  return view;
}
