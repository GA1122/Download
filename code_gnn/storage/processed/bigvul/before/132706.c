ChromotingInstance::ChromotingInstance(PP_Instance pp_instance)
    : pp::Instance(pp_instance),
      initialized_(false),
      plugin_task_runner_(new PluginThreadTaskRunner(&plugin_thread_delegate_)),
      context_(plugin_task_runner_.get()),
      input_tracker_(&mouse_input_filter_),
      key_mapper_(&input_tracker_),
      cursor_setter_(this),
      empty_cursor_filter_(&cursor_setter_),
      text_input_controller_(this),
      use_async_pin_dialog_(false),
      weak_factory_(this) {
#if defined(OS_NACL)
  thread_task_runner_handle_.reset(
      new base::ThreadTaskRunnerHandle(plugin_task_runner_));
  thread_wrapper_ =
      jingle_glue::JingleThreadWrapper::WrapTaskRunner(plugin_task_runner_);
  media::InitializeCPUSpecificYUVConversions();

  ChromotingInstance::RegisterLogMessageHandler();
#else
  jingle_glue::JingleThreadWrapper::EnsureForCurrentMessageLoop();
#endif

#if defined(OS_NACL)
  nacl_io_init_ppapi(pp_instance, pp::Module::Get()->get_browser_interface());
  mount("", "/etc", "memfs", 0, "");
  mount("", "/usr", "memfs", 0, "");
#endif

  RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE | PP_INPUTEVENT_CLASS_WHEEL);
  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);

  text_input_controller_.SetTextInputType(PP_TEXTINPUT_TYPE_NONE);

  RegisterLoggingInstance();

#if defined(USE_OPENSSL)
  char random_seed[kRandomSeedSize];
  crypto::RandBytes(random_seed, sizeof(random_seed));
  rtc::InitRandom(random_seed, sizeof(random_seed));
#else
  rtc::InitializeSSL();
#endif   

  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetInteger("apiVersion", kApiVersion);
  data->SetString("apiFeatures", kApiFeatures);
  data->SetInteger("apiMinVersion", kApiMinMessagingVersion);
  data->SetString("requestedCapabilities", kRequestedCapabilities);
  data->SetString("supportedCapabilities", kSupportedCapabilities);

  PostLegacyJsonMessage("hello", data.Pass());
}
