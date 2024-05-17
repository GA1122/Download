void AutomationProvider::SetExpectedTabCount(size_t expected_tabs) {
  if (expected_tabs == 0)
    OnInitialLoadsComplete();
  else
    initial_load_observer_.reset(new InitialLoadObserver(expected_tabs, this));
}
