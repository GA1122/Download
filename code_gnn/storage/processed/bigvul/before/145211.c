void Dispatcher::DidCreateScriptContext(
    blink::WebLocalFrame* frame,
    const v8::Local<v8::Context>& v8_context,
    int extension_group,
    int world_id) {
  const base::TimeTicks start_time = base::TimeTicks::Now();

  ScriptContext* context = script_context_set_->Register(
      frame, v8_context, extension_group, world_id);

  if (context->context_type() == Feature::CONTENT_SCRIPT_CONTEXT)
    InitOriginPermissions(context->extension());

  {
    scoped_ptr<ModuleSystem> module_system(
        new ModuleSystem(context, &source_map_));
    context->set_module_system(std::move(module_system));
  }
  ModuleSystem* module_system = context->module_system();

  ModuleSystem::NativesEnabledScope natives_enabled_scope(module_system);

  RegisterNativeHandlers(module_system, context);

  if (context->extension()) {
    v8::Local<v8::Object> chrome = AsObjectOrEmpty(GetOrCreateChrome(context));
    if (!chrome.IsEmpty())
      module_system->SetLazyField(chrome, "Event", kEventBindings, "Event");
  }

  UpdateBindingsForContext(context);

  bool is_within_platform_app = IsWithinPlatformApp();
  if (is_within_platform_app) {
    module_system->Require("platformApp");
  }

  RequireGuestViewModules(context);
  delegate_->RequireAdditionalModules(context, is_within_platform_app);

  const base::TimeDelta elapsed = base::TimeTicks::Now() - start_time;
  switch (context->context_type()) {
    case Feature::UNSPECIFIED_CONTEXT:
      UMA_HISTOGRAM_TIMES("Extensions.DidCreateScriptContext_Unspecified",
                          elapsed);
      break;
    case Feature::BLESSED_EXTENSION_CONTEXT:
      UMA_HISTOGRAM_TIMES("Extensions.DidCreateScriptContext_Blessed", elapsed);
      break;
    case Feature::UNBLESSED_EXTENSION_CONTEXT:
      UMA_HISTOGRAM_TIMES("Extensions.DidCreateScriptContext_Unblessed",
                          elapsed);
      break;
    case Feature::CONTENT_SCRIPT_CONTEXT:
      UMA_HISTOGRAM_TIMES("Extensions.DidCreateScriptContext_ContentScript",
                          elapsed);
      break;
    case Feature::WEB_PAGE_CONTEXT:
      UMA_HISTOGRAM_TIMES("Extensions.DidCreateScriptContext_WebPage", elapsed);
      break;
    case Feature::BLESSED_WEB_PAGE_CONTEXT:
      UMA_HISTOGRAM_TIMES("Extensions.DidCreateScriptContext_BlessedWebPage",
                          elapsed);
      break;
    case Feature::WEBUI_CONTEXT:
      UMA_HISTOGRAM_TIMES("Extensions.DidCreateScriptContext_WebUI", elapsed);
      break;
    case Feature::SERVICE_WORKER_CONTEXT:
      NOTREACHED();
      break;
  }

  VLOG(1) << "Num tracked contexts: " << script_context_set_->size();
}
