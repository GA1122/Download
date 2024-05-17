void Dispatcher::DidInitializeServiceWorkerContextOnWorkerThread(
    v8::Local<v8::Context> v8_context,
    const GURL& url) {
  const base::TimeTicks start_time = base::TimeTicks::Now();

  if (!url.SchemeIs(kExtensionScheme) &&
      !url.SchemeIs(kExtensionResourceScheme)) {
    return;
  }

  const Extension* extension =
      RendererExtensionRegistry::Get()->GetExtensionOrAppByURL(url);

  if (!extension) {
    return;
  }

  ScriptContext* context = new ScriptContext(
      v8_context, nullptr, extension, Feature::SERVICE_WORKER_CONTEXT,
      extension, Feature::SERVICE_WORKER_CONTEXT);
  context->set_url(url);

  g_worker_script_context_set.Get().Insert(make_scoped_ptr(context));

  v8::Isolate* isolate = context->isolate();

  base::StringPiece script_resource =
      ResourceBundle::GetSharedInstance().GetRawDataResource(
          IDR_SERVICE_WORKER_BINDINGS_JS);
  v8::Local<v8::String> script = v8::String::NewExternal(
      isolate, new StaticV8ExternalOneByteStringResource(script_resource));

  v8::Local<v8::Function> main_function;
  {
    v8::Local<v8::Value> result = context->RunScript(
        v8_helpers::ToV8StringUnsafe(isolate, "service_worker"), script,
        base::Bind(&CrashOnException));
    CHECK(result->IsFunction());
    main_function = result.As<v8::Function>();
  }

  LoggingNativeHandler* logging = new LoggingNativeHandler(context);
  context->AddInvalidationObserver(
      base::Bind(&NativeHandler::Invalidate, base::Owned(logging)));

  v8::Local<v8::Value> args[] = {
      v8_helpers::ToV8StringUnsafe(
          isolate, BackgroundInfo::GetBackgroundURL(extension).spec()),
      WakeEventPage::Get()->GetForContext(context),
      logging->NewInstance(),
  };
  context->CallFunction(main_function, arraysize(args), args);

  const base::TimeDelta elapsed = base::TimeTicks::Now() - start_time;
  UMA_HISTOGRAM_TIMES(
      "Extensions.DidInitializeServiceWorkerContextOnWorkerThread", elapsed);
}
