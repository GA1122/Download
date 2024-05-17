void ChromeContentRendererClient::RenderViewCreated(
    content::RenderView* render_view) {
  ContentSettingsObserver* content_settings =
      new ContentSettingsObserver(render_view);
  if (chrome_observer_.get()) {
    content_settings->SetContentSettingRules(
        chrome_observer_->content_setting_rules());
  }
  new ExtensionHelper(render_view, extension_dispatcher_.get());
  new PageLoadHistograms(render_view, histogram_snapshots_.get());
  new PrintWebViewHelper(render_view);
  new SearchBox(render_view);
  new SpellCheckProvider(render_view, this);
#if defined(ENABLE_SAFE_BROWSING)
  safe_browsing::MalwareDOMDetails::Create(render_view);
#endif

  PasswordAutofillManager* password_autofill_manager =
      new PasswordAutofillManager(render_view);
  AutofillAgent* autofill_agent = new AutofillAgent(render_view,
                                                    password_autofill_manager);
  PageClickTracker* page_click_tracker = new PageClickTracker(render_view);
  page_click_tracker->AddListener(password_autofill_manager);
  page_click_tracker->AddListener(autofill_agent);

  TranslateHelper* translate = new TranslateHelper(render_view);
  new ChromeRenderViewObserver(
      render_view, content_settings, chrome_observer_.get(),
      extension_dispatcher_.get(), translate);

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDomAutomationController)) {
    new AutomationRendererHelper(render_view);
  }
  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnablePasswordGeneration)) {
    new PasswordGenerationManager(render_view);
  }
}
