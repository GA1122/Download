Dispatcher::Dispatcher(DispatcherDelegate* delegate)
    : delegate_(delegate),
      content_watcher_(new ContentWatcher()),
      source_map_(&ResourceBundle::GetSharedInstance()),
      v8_schema_registry_(new V8SchemaRegistry),
      user_script_set_manager_observer_(this),
      webrequest_used_(false) {
  const base::CommandLine& command_line =
      *(base::CommandLine::ForCurrentProcess());
  set_idle_notifications_ =
      command_line.HasSwitch(switches::kExtensionProcess) ||
      command_line.HasSwitch(::switches::kSingleProcess);

  if (set_idle_notifications_) {
    RenderThread::Get()->SetIdleNotificationDelayInMs(
        kInitialExtensionIdleHandlerDelayMs);
  }

  script_context_set_.reset(new ScriptContextSet(&active_extension_ids_));
  user_script_set_manager_.reset(new UserScriptSetManager());
  script_injection_manager_.reset(
      new ScriptInjectionManager(user_script_set_manager_.get()));
  user_script_set_manager_observer_.Add(user_script_set_manager_.get());
  request_sender_.reset(new RequestSender(this));
  PopulateSourceMap();
  WakeEventPage::Get()->Init(RenderThread::Get());

  RenderThread::Get()->RegisterExtension(SafeBuiltins::CreateV8Extension());

  using RegisterFunction = void (*)(const WebString&);
  RegisterFunction register_functions[] = {
      WebSecurityPolicy::registerURLSchemeAsSecure,
      WebSecurityPolicy::registerURLSchemeAsCORSEnabled,
      WebSecurityPolicy::registerURLSchemeAsBypassingContentSecurityPolicy,
      WebSecurityPolicy::registerURLSchemeAsSupportingFetchAPI,
      WebSecurityPolicy::registerURLSchemeAsFirstPartyWhenTopLevel,
  };

  WebString extension_scheme(base::ASCIIToUTF16(kExtensionScheme));
  WebString extension_resource_scheme(base::ASCIIToUTF16(
      kExtensionResourceScheme));
  for (RegisterFunction func : register_functions) {
    func(extension_scheme);
    func(extension_resource_scheme);
  }

  if (set_idle_notifications_) {
    forced_idle_timer_.reset(new base::RepeatingTimer);
    forced_idle_timer_->Start(
        FROM_HERE,
        base::TimeDelta::FromMilliseconds(kMaxExtensionIdleHandlerDelayMs),
        RenderThread::Get(),
        &RenderThread::IdleHandler);
  }

  for (const std::string& extension_id : active_extension_ids_) {
    const Extension* extension =
        RendererExtensionRegistry::Get()->GetByID(extension_id);
    CHECK(extension);
    InitOriginPermissions(extension);
  }

  EnableCustomElementWhiteList();
}
