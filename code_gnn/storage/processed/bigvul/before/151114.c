void DevToolsWindow::Show(const DevToolsToggleAction& action) {
  if (life_stage_ == kClosing)
    return;

  if (action.type() == DevToolsToggleAction::kNoOp)
    return;
  if (is_docked_) {
    DCHECK(can_dock_);
    Browser* inspected_browser = NULL;
    int inspected_tab_index = -1;
    FindInspectedBrowserAndTabIndex(GetInspectedWebContents(),
                                    &inspected_browser,
                                    &inspected_tab_index);
    DCHECK(inspected_browser);
    DCHECK(inspected_tab_index != -1);

    BrowserWindow* inspected_window = inspected_browser->window();
    main_web_contents_->SetDelegate(this);

    TabStripModel* tab_strip_model = inspected_browser->tab_strip_model();
    tab_strip_model->ActivateTabAt(inspected_tab_index, true);

    inspected_window->UpdateDevTools();
    main_web_contents_->SetInitialFocus();
    inspected_window->Show();
    main_web_contents_->SetInitialFocus();

    PrefsTabHelper::CreateForWebContents(main_web_contents_);
    main_web_contents_->GetRenderViewHost()->SyncRendererPrefs();

    DoAction(action);
    return;
  }

  bool should_show_window =
      !browser_ || (action.type() != DevToolsToggleAction::kInspect);

  if (!browser_)
    CreateDevToolsBrowser();

  if (should_show_window) {
    browser_->window()->Show();
    main_web_contents_->SetInitialFocus();
  }
  if (toolbox_web_contents_)
    UpdateBrowserWindow();

  DoAction(action);
}
