bool Plugin::StartProxiedExecution(NaClSrpcChannel* srpc_channel,
                                   ErrorInfo* error_info) {
  PLUGIN_PRINTF(("Plugin::StartProxiedExecution (srpc_channel=%p)\n",
                 static_cast<void*>(srpc_channel)));

  HistogramStartupTimeMedium(
      "NaCl.Perf.StartupTime.NaClOverhead",
      static_cast<float>(NaClGetTimeOfDayMicroseconds() - init_time_)
          / NACL_MICROS_PER_MILLI);

  NaClSrpcService* client_service = srpc_channel->client;
  if (NaClSrpcServiceMethodIndex(client_service,
                                 "PPP_InitializeModule:ihs:i") ==
      kNaClSrpcInvalidMethodIndex) {
    error_info->SetReport(
        ERROR_START_PROXY_CHECK_PPP,
        "could not find PPP_InitializeModule() - toolchain version mismatch?");
    PLUGIN_PRINTF(("Plugin::StartProxiedExecution (%s)\n",
                   error_info->message().c_str()));
    return false;
  }
  nacl::scoped_ptr<BrowserPpp> ppapi_proxy(new BrowserPpp(srpc_channel, this));
  PLUGIN_PRINTF(("Plugin::StartProxiedExecution (ppapi_proxy=%p)\n",
                 static_cast<void*>(ppapi_proxy.get())));
  if (ppapi_proxy.get() == NULL) {
    error_info->SetReport(ERROR_START_PROXY_ALLOC,
                          "could not allocate proxy memory.");
    return false;
  }
  pp::Module* module = pp::Module::Get();
  PLUGIN_PRINTF(("Plugin::StartProxiedExecution (module=%p)\n",
                 static_cast<void*>(module)));
  CHECK(module != NULL);   
  int32_t pp_error =
      ppapi_proxy->InitializeModule(module->pp_module(),
                                    module->get_browser_interface());
  PLUGIN_PRINTF(("Plugin::StartProxiedExecution (pp_error=%"
                 NACL_PRId32")\n", pp_error));
  if (pp_error != PP_OK) {
    error_info->SetReport(ERROR_START_PROXY_MODULE,
                          "could not initialize module.");
    return false;
  }
  const PPP_Instance* instance_interface =
      ppapi_proxy->ppp_instance_interface();
  PLUGIN_PRINTF(("Plugin::StartProxiedExecution (ppp_instance=%p)\n",
                 static_cast<const void*>(instance_interface)));
  CHECK(instance_interface != NULL);   
  PP_Bool did_create = instance_interface->DidCreate(
      pp_instance(),
      argc(),
      const_cast<const char**>(argn()),
      const_cast<const char**>(argv()));
  PLUGIN_PRINTF(("Plugin::StartProxiedExecution (did_create=%d)\n",
                 did_create));
  if (did_create == PP_FALSE) {
    error_info->SetReport(ERROR_START_PROXY_INSTANCE,
                          "could not create instance.");
    return false;
  }

  ppapi_proxy_ = ppapi_proxy.release();

  find_adapter_.reset(new FindAdapter(this));
  mouse_lock_adapter_.reset(new MouseLockAdapter(this));
  printing_adapter_.reset(new PrintingAdapter(this));
  selection_adapter_.reset(new SelectionAdapter(this));
  widget_client_adapter_.reset(new WidgetClientAdapter(this));
  zoom_adapter_.reset(new ZoomAdapter(this));

  if (!view_to_replay_.is_null()) {
    DidChangeView(view_to_replay_);
    view_to_replay_ = pp::View();
  }
  if (!document_load_to_replay_.is_null()) {
    HandleDocumentLoad(document_load_to_replay_);
    document_load_to_replay_ = pp::URLLoader();
  }
  bool is_valid_proxy = BrowserPpp::is_valid(ppapi_proxy_);
  PLUGIN_PRINTF(("Plugin::StartProxiedExecution (is_valid_proxy=%d)\n",
                 is_valid_proxy));
  if (!is_valid_proxy) {
    error_info->SetReport(ERROR_START_PROXY_CRASH,
                          "instance crashed after creation.");
  }
  return is_valid_proxy;
}
