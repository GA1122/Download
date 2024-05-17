 void ChromeContentRendererClient::RenderThreadStarted() {
   chrome_observer_.reset(new ChromeRenderProcessObserver());
   extension_dispatcher_.reset(new ExtensionDispatcher());
   histogram_snapshots_.reset(new RendererHistogramSnapshots());
   net_predictor_.reset(new RendererNetPredictor());
   spellcheck_.reset(new SpellCheck());
   visited_link_slave_.reset(new VisitedLinkSlave());
 #if defined(ENABLE_SAFE_BROWSING)
   phishing_classifier_.reset(safe_browsing::PhishingClassifierFilter::Create());
  #endif
  
    RenderThread* thread = RenderThread::current();
  thread->AddFilter(new DevToolsAgentFilter());
  
    thread->AddObserver(chrome_observer_.get());
    thread->AddObserver(extension_dispatcher_.get());
   thread->AddObserver(histogram_snapshots_.get());
 #if defined(ENABLE_SAFE_BROWSING)
   thread->AddObserver(phishing_classifier_.get());
 #endif
   thread->AddObserver(spellcheck_.get());
   thread->AddObserver(visited_link_slave_.get());
 
   thread->RegisterExtension(extensions_v8::ExternalExtension::Get());
   thread->RegisterExtension(extensions_v8::LoadTimesExtension::Get());
   thread->RegisterExtension(extensions_v8::SearchBoxExtension::Get());
   v8::Extension* search_extension = extensions_v8::SearchExtension::Get();
   if (search_extension)
     thread->RegisterExtension(search_extension);
 
   if (CommandLine::ForCurrentProcess()->HasSwitch(
           switches::kDomAutomationController)) {
     thread->RegisterExtension(DomAutomationV8Extension::Get());
   }
 
   if (CommandLine::ForCurrentProcess()->HasSwitch(
           switches::kEnableIPCFuzzing)) {
     thread->channel()->set_outgoing_message_filter(LoadExternalIPCFuzzer());
   }
   WebString chrome_ui_scheme(ASCIIToUTF16(chrome::kChromeUIScheme));
   WebSecurityPolicy::registerURLSchemeAsDisplayIsolated(chrome_ui_scheme);
 
   WebString dev_tools_scheme(ASCIIToUTF16(chrome::kChromeDevToolsScheme));
   WebSecurityPolicy::registerURLSchemeAsDisplayIsolated(dev_tools_scheme);
 
   WebString internal_scheme(ASCIIToUTF16(chrome::kChromeInternalScheme));
   WebSecurityPolicy::registerURLSchemeAsDisplayIsolated(internal_scheme);
 
   WebString extension_scheme(ASCIIToUTF16(chrome::kExtensionScheme));
   WebSecurityPolicy::registerURLSchemeAsSecure(extension_scheme);
 }