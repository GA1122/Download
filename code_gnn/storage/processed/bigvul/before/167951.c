inline LocalFrame::LocalFrame(LocalFrameClient* client,
                              Page& page,
                              FrameOwner* owner,
                              InterfaceRegistry* interface_registry)
    : Frame(client, page, owner, LocalWindowProxyManager::Create(*this)),
      frame_scheduler_(page.GetPageScheduler()
                           ? page.GetPageScheduler()->CreateFrameScheduler(
                                 client->GetFrameBlameContext(),
                                 IsMainFrame()
                                     ? FrameScheduler::FrameType::kMainFrame
                                     : FrameScheduler::FrameType::kSubframe)
                           : std::make_unique<EmptyFrameScheduler>()),
      loader_(this),
      navigation_scheduler_(NavigationScheduler::Create(this)),
      script_controller_(ScriptController::Create(
          *this,
          *static_cast<LocalWindowProxyManager*>(GetWindowProxyManager()))),
      editor_(Editor::Create(*this)),
      spell_checker_(SpellChecker::Create(*this)),
      selection_(FrameSelection::Create(*this)),
      event_handler_(new EventHandler(*this)),
      console_(FrameConsole::Create(*this)),
      input_method_controller_(InputMethodController::Create(*this)),
      text_suggestion_controller_(new TextSuggestionController(*this)),
      navigation_disable_count_(0),
      page_zoom_factor_(ParentPageZoomFactor(this)),
      text_zoom_factor_(ParentTextZoomFactor(this)),
      in_view_source_mode_(false),
      inspector_task_runner_(InspectorTaskRunner::Create(
          GetTaskRunner(TaskType::kInternalInspector))),
      interface_registry_(interface_registry) {
  if (IsLocalRoot()) {
    probe_sink_ = new CoreProbeSink();
    ad_tracker_ = new AdTracker(this);
    performance_monitor_ = new PerformanceMonitor(this);
    inspector_trace_events_ = new InspectorTraceEvents();
    probe_sink_->addInspectorTraceEvents(inspector_trace_events_);
  } else {
    UpdateInertIfPossible();
    UpdateInheritedEffectiveTouchActionIfPossible();
    probe_sink_ = LocalFrameRoot().probe_sink_;
    ad_tracker_ = LocalFrameRoot().ad_tracker_;
    performance_monitor_ = LocalFrameRoot().performance_monitor_;
  }
  idleness_detector_ = new IdlenessDetector(this);
  inspector_task_runner_->InitIsolate(V8PerIsolateData::MainThreadIsolate());

  SetIsAdSubframeIfNecessary();
}
