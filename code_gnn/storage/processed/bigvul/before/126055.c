TabCountChangeObserver::TabCountChangeObserver(AutomationProvider* automation,
                                               Browser* browser,
                                               IPC::Message* reply_message,
                                               int target_tab_count)
    : automation_(automation->AsWeakPtr()),
      reply_message_(reply_message),
      tab_strip_model_(browser->tab_strip_model()),
      target_tab_count_(target_tab_count) {
  tab_strip_model_->AddObserver(this);
  CheckTabCount();
}
