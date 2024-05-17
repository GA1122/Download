Tab::Tab(TabController* controller)
    : controller_(controller),
      title_(new views::Label()),
      title_animation_(this) {
  DCHECK(controller);

  tab_style_ = TabStyleViews::CreateForTab(this);

  set_notify_enter_exit_on_child(true);

  SetID(VIEW_ID_TAB);

  SetBorder(views::CreateEmptyBorder(tab_style()->GetContentsInsets()));

  title_->SetHorizontalAlignment(gfx::ALIGN_TO_HEAD);
  title_->SetElideBehavior(gfx::FADE_TAIL);
  title_->SetHandlesTooltips(false);
  title_->SetAutoColorReadabilityEnabled(false);
  title_->SetText(CoreTabHelper::GetDefaultTitle());
  AddChildView(title_);

  SetEventTargeter(std::make_unique<views::ViewTargeter>(this));

  icon_ = new TabIcon;
  AddChildView(icon_);

  alert_indicator_ = new AlertIndicator(this);
  AddChildView(alert_indicator_);

  close_button_ = new TabCloseButton(
      this, base::BindRepeating(&TabController::OnMouseEventInTab,
                                base::Unretained(controller_)));
  AddChildView(close_button_);
  close_button_->AddObserver(this);

  set_context_menu_controller(this);

  title_animation_.SetDuration(base::TimeDelta::FromMilliseconds(100));

  SetFocusBehavior(FocusBehavior::ACCESSIBLE_ONLY);
  focus_ring_ = views::FocusRing::Install(this);
}
