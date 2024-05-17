DevToolsWindow::DevToolsWindow(FrontendType frontend_type,
                               Profile* profile,
                               WebContents* main_web_contents,
                               DevToolsUIBindings* bindings,
                               WebContents* inspected_web_contents,
                               bool can_dock)
    : frontend_type_(frontend_type),
      profile_(profile),
      main_web_contents_(main_web_contents),
      toolbox_web_contents_(nullptr),
      bindings_(bindings),
      browser_(nullptr),
      is_docked_(true),
      can_dock_(can_dock),
      close_on_detach_(true),
      life_stage_(can_dock ? kNotLoaded : kIsDockedSet),
      action_on_load_(DevToolsToggleAction::NoOp()),
      intercepted_page_beforeunload_(false),
      ready_for_test_(false) {
  main_web_contents_->SetDelegate(this);
  bindings_->SetDelegate(this);
  data_use_measurement::DataUseWebContentsObserver::CreateForWebContents(
      main_web_contents_);
  zoom::ZoomController::CreateForWebContents(main_web_contents_);
  zoom::ZoomController::FromWebContents(main_web_contents_)
      ->SetShowsNotificationBubble(false);

  g_instances.Get().push_back(this);

  if (inspected_web_contents)
    inspected_contents_observer_.reset(
        new ObserverWithAccessor(inspected_web_contents));

  if (can_dock_ && inspected_web_contents) {
    content::RenderWidgetHostView* inspected_view =
        inspected_web_contents->GetRenderWidgetHostView();
    if (inspected_view && main_web_contents_->GetRenderWidgetHostView()) {
      gfx::Size size = inspected_view->GetViewBounds().size();
      main_web_contents_->GetRenderWidgetHostView()->SetSize(size);
    }
  }

  event_forwarder_.reset(new DevToolsEventForwarder(this));

  task_manager::WebContentsTags::CreateForDevToolsContents(main_web_contents_);

  std::vector<base::Callback<void(DevToolsWindow*)>> copy(
      g_creation_callbacks.Get());
  for (const auto& callback : copy)
    callback.Run(this);
}
