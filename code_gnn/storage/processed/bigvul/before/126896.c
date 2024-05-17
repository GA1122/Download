BrowserTabStripController::BrowserTabStripController(Browser* browser,
                                                     TabStripModel* model)
    : model_(model),
      tabstrip_(NULL),
      browser_(browser),
      hover_tab_selector_(model) {
  model_->AddObserver(this);

  local_pref_registrar_.Init(g_browser_process->local_state());
  local_pref_registrar_.Add(prefs::kTabStripLayoutType, this);
}
