AutomationProvider::AutomationProvider(Profile* profile)
    : profile_(profile),
      reply_message_(NULL),
      is_connected_(false),
      initial_loads_complete_(false) {
  TRACE_EVENT_BEGIN("AutomationProvider::AutomationProvider", 0, "");

  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  browser_tracker_.reset(new AutomationBrowserTracker(this));
  extension_tracker_.reset(new AutomationExtensionTracker(this));
  tab_tracker_.reset(new AutomationTabTracker(this));
  window_tracker_.reset(new AutomationWindowTracker(this));
  autocomplete_edit_tracker_.reset(
      new AutomationAutocompleteEditTracker(this));
  new_tab_ui_load_observer_.reset(new NewTabUILoadObserver(this));
  dom_operation_observer_.reset(new DomOperationMessageSender(this));
  metric_event_duration_observer_.reset(new MetricEventDurationObserver());
  extension_test_result_observer_.reset(
      new ExtensionTestResultNotificationObserver(this));
  g_browser_process->AddRefModule();

  TRACE_EVENT_END("AutomationProvider::AutomationProvider", 0, "");
}
