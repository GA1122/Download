void Document::Shutdown() {
  TRACE_EVENT0("blink", "Document::shutdown");
  CHECK(!frame_ || frame_->Tree().ChildCount() == 0);
  if (!IsActive())
    return;

  FrameNavigationDisabler navigation_disabler(*frame_);
  HTMLFrameOwnerElement::PluginDisposeSuspendScope suspend_plugin_dispose;
  ScriptForbiddenScope forbid_script;

  lifecycle_.AdvanceTo(DocumentLifecycle::kStopping);
  View()->Dispose();
  CHECK(!View()->IsAttached());

  HTMLFrameOwnerElement* owner_element = frame_->DeprecatedLocalOwner();
  if (owner_element)
    owner_element->SetEmbeddedContentView(nullptr);

  markers_->PrepareForDestruction();

  if (GetPage())
    GetPage()->DocumentDetached(this);
  probe::documentDetached(this);

  if (frame_->Client()->GetSharedWorkerRepositoryClient())
    frame_->Client()->GetSharedWorkerRepositoryClient()->DocumentDetached(this);

  if (scripted_animation_controller_)
    scripted_animation_controller_->ClearDocumentPointer();
  scripted_animation_controller_.Clear();

  scripted_idle_task_controller_.Clear();

  if (SvgExtensions())
    AccessSVGExtensions().PauseAnimations();

  if (dom_window_)
    dom_window_->ClearEventQueue();

  if (layout_view_)
    layout_view_->SetIsInWindow(false);

  if (RegistrationContext())
    RegistrationContext()->DocumentWasDetached();

  MutationObserver::CleanSlotChangeList(*this);

  hover_element_ = nullptr;
  active_element_ = nullptr;
  autofocus_element_ = nullptr;

  if (focused_element_.Get()) {
    Element* old_focused_element = focused_element_;
    focused_element_ = nullptr;
    if (GetPage())
      GetPage()->GetChromeClient().FocusedNodeChanged(old_focused_element,
                                                      nullptr);
  }
  sequential_focus_navigation_starting_point_ = nullptr;

  if (this == &AxObjectCacheOwner())
    ClearAXObjectCache();

  layout_view_ = nullptr;
  ContainerNode::DetachLayoutTree();
  CHECK(!View()->IsAttached());

  if (this != &AxObjectCacheOwner()) {
    if (AXObjectCache* cache = ExistingAXObjectCache()) {
      for (Node& node : NodeTraversal::DescendantsOf(*this)) {
        cache->Remove(&node);
      }
    }
  }

  GetStyleEngine().DidDetach();

  GetPage()->GetEventHandlerRegistry().DocumentDetached(*this);

  DocumentShutdownNotifier::NotifyContextDestroyed();
  SynchronousMutationNotifier::NotifyContextDestroyed();

  if (!Loader())
    fetcher_->ClearContext();
  if (imports_controller_) {
    imports_controller_->Dispose();
    ClearImportsController();
  }

  timers_.SetTimerTaskRunner(
      Platform::Current()->CurrentThread()->Scheduler()->TimerTaskRunner());

  if (media_query_matcher_)
    media_query_matcher_->DocumentDetached();

  lifecycle_.AdvanceTo(DocumentLifecycle::kStopped);
  CHECK(!View()->IsAttached());

  ExecutionContext::NotifyContextDestroyed();
  CHECK(!View()->IsAttached());

  frame_ = nullptr;

  document_outlive_time_reporter_ =
      WTF::WrapUnique(new DocumentOutliveTimeReporter(this));
}
