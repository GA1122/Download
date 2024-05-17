Document::Document(const DocumentInit& initializer,
                   DocumentClassFlags document_classes)
    : ContainerNode(nullptr, kCreateDocument),
      TreeScope(*this),
      ExecutionContext(V8PerIsolateData::MainThreadIsolate()),
      evaluate_media_queries_on_style_recalc_(false),
      pending_sheet_layout_(kNoLayoutWithPendingSheets),
      frame_(initializer.GetFrame()),
      dom_window_(frame_ ? frame_->DomWindow() : nullptr),
      imports_controller_(initializer.ImportsController()),
      context_document_(initializer.ContextDocument()),
      context_features_(ContextFeatures::DefaultSwitch()),
      well_formed_(false),
      printing_(kNotPrinting),
      compatibility_mode_(kNoQuirksMode),
      compatibility_mode_locked_(false),
      has_autofocused_(false),
      last_focus_type_(kWebFocusTypeNone),
      had_keyboard_event_(false),
      clear_focused_element_timer_(
          GetTaskRunner(TaskType::kInternalUserInteraction),
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
      contains_validity_style_rules_(false),
      contains_plugins_(false),
      ignore_destructive_write_count_(0),
      throw_on_dynamic_markup_insertion_count_(0),
      ignore_opens_during_unload_count_(0),
      markers_(MakeGarbageCollected<DocumentMarkerController>(*this)),
      update_focus_appearance_timer_(
          GetTaskRunner(TaskType::kInternalUserInteraction),
          this,
          &Document::UpdateFocusAppearanceTimerFired),
      css_target_(nullptr),
      was_discarded_(false),
      load_event_progress_(kLoadEventCompleted),
      is_freezing_in_progress_(false),
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
      is_srcdoc_document_(initializer.IsSrcdocDocument()),
      is_mobile_document_(false),
      layout_view_(nullptr),
      has_fullscreen_supplement_(false),
      load_event_delay_count_(0),
      load_event_delay_timer_(GetTaskRunner(TaskType::kNetworking),
                              this,
                              &Document::LoadEventDelayTimerFired),
      plugin_loading_timer_(GetTaskRunner(TaskType::kInternalLoading),
                            this,
                            &Document::PluginLoadingTimerFired),
      document_timing_(*this),
      write_recursion_is_too_deep_(false),
      write_recursion_depth_(0),
      registration_context_(initializer.RegistrationContext(this)),
      element_data_cache_clear_timer_(
          GetTaskRunner(TaskType::kInternalUserInteraction),
          this,
          &Document::ElementDataCacheClearTimerFired),
      timeline_(DocumentTimeline::Create(this)),
      pending_animations_(MakeGarbageCollected<PendingAnimations>(*this)),
      worklet_animation_controller_(
          MakeGarbageCollected<WorkletAnimationController>(this)),
      template_document_host_(nullptr),
      did_associate_form_controls_timer_(
          GetTaskRunner(TaskType::kInternalLoading),
          this,
          &Document::DidAssociateFormControlsTimerFired),
      timers_(GetTaskRunner(TaskType::kJavascriptTimer)),
      has_viewport_units_(false),
      parser_sync_policy_(kAllowAsynchronousParsing),
      node_count_(0),
      logged_field_edit_(false),
      secure_context_state_(SecureContextState::kUnknown),
      ukm_source_id_(ukm::UkmRecorder::GetNewSourceID()),
#if DCHECK_IS_ON()
      slot_assignment_recalc_forbidden_recursion_depth_(0),
#endif
      needs_to_record_ukm_outlive_time_(false),
      viewport_data_(MakeGarbageCollected<ViewportData>(*this)),
      agent_cluster_id_(base::UnguessableToken::Create()),
      parsed_feature_policies_(
          static_cast<int>(mojom::FeaturePolicyFeature::kMaxValue) + 1),
      potentially_violated_features_(
          static_cast<size_t>(mojom::FeaturePolicyFeature::kMaxValue) + 1U),
      isolated_world_csp_map_(
          MakeGarbageCollected<
              HeapHashMap<int, Member<ContentSecurityPolicy>>>()) {
  if (frame_) {
    DCHECK(frame_->GetPage());
    ProvideContextFeaturesToDocumentFrom(*this, *frame_->GetPage());
    fetcher_ = FrameFetchContext::CreateFetcherForCommittedDocument(
        *frame_->Loader().GetDocumentLoader(), *this);
    CustomElementRegistry* registry =
        frame_->DomWindow() ? frame_->DomWindow()->MaybeCustomElements()
                            : nullptr;
    if (registry && registration_context_)
      registry->Entangle(registration_context_);
  } else if (imports_controller_) {
    fetcher_ = FrameFetchContext::CreateFetcherForImportedDocument(this);
  } else {
    fetcher_ = MakeGarbageCollected<ResourceFetcher>(ResourceFetcherInit(
        *MakeGarbageCollected<NullResourceFetcherProperties>(),
        &FetchContext::NullInstance(), GetTaskRunner(TaskType::kNetworking)));
  }
  DCHECK(fetcher_);

  root_scroller_controller_ = RootScrollerController::Create(*this);

  if (initializer.ShouldSetURL()) {
    SetURL(initializer.Url());
  } else {
    UpdateBaseURL();
  }

  InitSecurityContext(initializer);
  if (frame_)
    frame_->Client()->DidSetFramePolicyHeaders(GetSandboxFlags(), {});

  InitDNSPrefetch();

  InstanceCounters::IncrementCounter(InstanceCounters::kDocumentCounter);

  lifecycle_.AdvanceTo(DocumentLifecycle::kInactive);

  style_engine_ = StyleEngine::Create(*this);

  DCHECK(!ParentDocument() || !ParentDocument()->IsContextPaused());

#ifndef NDEBUG
  liveDocumentSet().insert(this);
#endif
}
