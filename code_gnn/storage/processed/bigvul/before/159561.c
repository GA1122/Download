Document::Document(const DocumentInit& initializer,
                   DocumentClassFlags document_classes)
    : ContainerNode(nullptr, kCreateDocument),
      TreeScope(*this),
      has_nodes_with_placeholder_style_(false),
      evaluate_media_queries_on_style_recalc_(false),
      pending_sheet_layout_(kNoLayoutWithPendingSheets),
      frame_(initializer.GetFrame()),
      dom_window_(frame_ ? frame_->DomWindow() : nullptr),
      imports_controller_(initializer.ImportsController()),
      context_document_(initializer.ContextDocument()),
      context_features_(ContextFeatures::DefaultSwitch()),
      well_formed_(false),
      printing_(kNotPrinting),
      paginated_for_screen_(false),
      compatibility_mode_(kNoQuirksMode),
      compatibility_mode_locked_(false),
      has_autofocused_(false),
      clear_focused_element_timer_(GetTaskRunner(TaskType::kUnspecedTimer),
                                   this,
                                   &Document::ClearFocusedElementTimerFired),
      dom_tree_version_(++global_tree_version_),
      style_version_(0),
      listener_types_(0),
      mutation_observer_types_(0),
      visited_link_state_(VisitedLinkState::Create(*this)),
      visually_ordered_(false),
      ready_state_(kComplete),
      parsing_state_(kFinishedParsing),
      goto_anchor_needed_after_stylesheets_load_(false),
      contains_validity_style_rules_(false),
      contains_plugins_(false),
      ignore_destructive_write_count_(0),
      throw_on_dynamic_markup_insertion_count_(0),
      markers_(new DocumentMarkerController(*this)),
      update_focus_appearance_timer_(
          GetTaskRunner(TaskType::kUnspecedTimer),
          this,
          &Document::UpdateFocusAppearanceTimerFired),
      css_target_(nullptr),
      load_event_progress_(kLoadEventCompleted),
      start_time_(CurrentTime()),
      script_runner_(ScriptRunner::Create(this)),
      xml_version_("1.0"),
      xml_standalone_(kStandaloneUnspecified),
      has_xml_declaration_(0),
      design_mode_(false),
      is_running_exec_command_(false),
      has_annotated_regions_(false),
      annotated_regions_dirty_(false),
      document_classes_(document_classes),
      is_view_source_(false),
      saw_elements_in_known_namespaces_(false),
      is_srcdoc_document_(initializer.ShouldTreatURLAsSrcdocDocument()),
      is_mobile_document_(false),
      layout_view_(nullptr),
      has_fullscreen_supplement_(false),
      load_event_delay_count_(0),
      load_event_delay_timer_(GetTaskRunner(TaskType::kNetworking),
                              this,
                              &Document::LoadEventDelayTimerFired),
      plugin_loading_timer_(GetTaskRunner(TaskType::kUnspecedLoading),
                            this,
                            &Document::PluginLoadingTimerFired),
      document_timing_(*this),
      write_recursion_is_too_deep_(false),
      write_recursion_depth_(0),
      registration_context_(initializer.RegistrationContext(this)),
      element_data_cache_clear_timer_(
          GetTaskRunner(TaskType::kUnspecedTimer),
          this,
          &Document::ElementDataCacheClearTimerFired),
      timeline_(DocumentTimeline::Create(this)),
      pending_animations_(new PendingAnimations(*this)),
      worklet_animation_controller_(new WorkletAnimationController(this)),
      template_document_host_(nullptr),
      did_associate_form_controls_timer_(
          GetTaskRunner(TaskType::kUnspecedLoading),
          this,
          &Document::DidAssociateFormControlsTimerFired),
      timers_(GetTaskRunner(TaskType::kJavascriptTimer)),
      has_viewport_units_(false),
      parser_sync_policy_(kAllowAsynchronousParsing),
      node_count_(0),
      would_load_reason_(WouldLoadReason::kInvalid),
      password_count_(0),
      logged_field_edit_(false),
      engagement_level_(mojom::blink::EngagementLevel::NONE) {
  if (frame_) {
    DCHECK(frame_->GetPage());
    ProvideContextFeaturesToDocumentFrom(*this, *frame_->GetPage());

    fetcher_ = frame_->Loader().GetDocumentLoader()->Fetcher();
    FrameFetchContext::ProvideDocumentToContext(fetcher_->Context(), this);

    CustomElementRegistry* registry =
        frame_->DomWindow() ? frame_->DomWindow()->MaybeCustomElements()
                            : nullptr;
    if (registry && registration_context_)
      registry->Entangle(registration_context_);
  } else if (imports_controller_) {
    fetcher_ = FrameFetchContext::CreateFetcherFromDocument(this);
  } else {
    fetcher_ = ResourceFetcher::Create(nullptr);
  }
  DCHECK(fetcher_);

  root_scroller_controller_ = RootScrollerController::Create(*this);

  if (initializer.ShouldSetURL()) {
    SetURL(initializer.Url());
  } else {
    UpdateBaseURL();
  }

  InitSecurityContext(initializer);

  InitDNSPrefetch();

  InstanceCounters::IncrementCounter(InstanceCounters::kDocumentCounter);

  lifecycle_.AdvanceTo(DocumentLifecycle::kInactive);

  style_engine_ = StyleEngine::Create(*this);

  DCHECK(!ParentDocument() || !ParentDocument()->IsContextPaused());

#ifndef NDEBUG
  liveDocumentSet().insert(this);
#endif
}
