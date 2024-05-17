views::View* LauncherView::CreateViewForItem(const LauncherItem& item) {
  views::View* view = NULL;
  switch (item.type) {
    case TYPE_TABBED: {
      TabbedLauncherButton* button =
          TabbedLauncherButton::Create(
              this,
              this,
              item.is_incognito ?
                  TabbedLauncherButton::STATE_INCOGNITO :
                  TabbedLauncherButton::STATE_NOT_INCOGNITO);
      button->SetTabImage(item.image);
      ReflectItemStatus(item, button);
      view = button;
      break;
    }

    case TYPE_APP_SHORTCUT:
    case TYPE_PLATFORM_APP:
    case TYPE_APP_PANEL: {
      LauncherButton* button = LauncherButton::Create(this, this);
      button->SetImage(item.image);
      ReflectItemStatus(item, button);
      view = button;
      break;
    }

    case TYPE_APP_LIST: {
      ResourceBundle& rb = ResourceBundle::GetSharedInstance();
      AppListButton* button = new AppListButton(this, this);
      button->SetImage(
          views::CustomButton::BS_NORMAL,
          rb.GetImageNamed(IDR_AURA_LAUNCHER_ICON_APPLIST).ToImageSkia());
      button->SetImage(
          views::CustomButton::BS_HOT,
          rb.GetImageNamed(IDR_AURA_LAUNCHER_ICON_APPLIST_HOT).
              ToImageSkia());
      button->SetImage(
          views::CustomButton::BS_PUSHED,
          rb.GetImageNamed(IDR_AURA_LAUNCHER_ICON_APPLIST_PUSHED).
              ToImageSkia());
      button->SetAccessibleName(
          l10n_util::GetStringUTF16(IDS_AURA_APP_LIST_TITLE));
      view = button;
      break;
    }

    case TYPE_BROWSER_SHORTCUT: {
      ResourceBundle& rb = ResourceBundle::GetSharedInstance();
      LauncherButton* button = LauncherButton::Create(this, this);
      int image_id = delegate_ ?
          delegate_->GetBrowserShortcutResourceId() :
          IDR_AURA_LAUNCHER_BROWSER_SHORTCUT;
      button->SetImage(*rb.GetImageNamed(image_id).ToImageSkia());
      view = button;
      break;
    }

    default:
      break;
  }
  view->set_context_menu_controller(this);

  DCHECK(view);
  ConfigureChildView(view);
  return view;
}
