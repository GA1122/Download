void BrowserTabStripController::InitFromModel(TabStrip* tabstrip) {
  tabstrip_ = tabstrip;

  UpdateLayoutType();

  for (int i = 0; i < model_->count(); ++i)
    AddTab(model_->GetWebContentsAt(i), i, model_->active_index() == i);
}
