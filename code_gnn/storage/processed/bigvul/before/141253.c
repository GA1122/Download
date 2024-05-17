void Document::Shutdown() {
  if (num_canvases_ > 0)
    UMA_HISTOGRAM_COUNTS_100("Blink.Canvas.NumCanvasesPerPage", num_canvases_);
  TRACE_EVENT0("blink", "Document::shutdown");
  CHECK(!frame_ || frame_->Tree().ChildCount() == 0);
  if (!IsActive())
    return;

  GetViewportData().Shutdown();

  FrameNavigationDisabler navigation_disabler(*frame_);
  HTMLFrameOwnerElement::PluginDisposeSuspendScope suspend_plugin_dispose;
  ScriptForbiddenScope forbid_script;

  lifecycle_.AdvanceTo(DocumentLifecycle::kStopping);
  View()->Dispose();
  CHECK(!View()->IsAttached());

  HTMLFrameOwnerElement* owner_element = frame_->DeprecatedLocalOwner();

  if (owner_element && !frame_->IsProvisional())
    owner_element->SetEmbeddedContentView(nullptr);

  markers_->PrepareForDestruction();

  if (GetPage()) {
    GetPage()->DocumentDetached(this);
    if (RuntimeEnabledFeatures::CompositeAfterPaintEnabled()) {
      if (auto* compositor_timeline = Timeline().CompositorTimeline()) {
        GetPage()->GetChromeClient().DetachCompositorAnimationTimeline(
            compositor_timeline, frame_.Get());
      }
    }
  }

  probe::DocumentDetached(this);

  if (scripted_animation_controller_)
    scripted_animation_controller_->ClearDocumentPointer();
  scripted_animation_controller_.Clear();

  scripted_idle_task_controller_.Clear();

  if (SvgExtensions())
    AccessSVGExtensions().PauseAnimations();

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

  if (this == &AXObjectCacheOwner()) {
    ax_contexts_.clear();
    ClearAXObjectCache();
  }
  computed_node_mapping_.clear();

  layout_view_ = nullptr;
  ContainerNode::DetachLayoutTree();
  CHECK(!View()->IsAttached());

  if (this != &AXObjectCacheOwner()) {
    if (AXObjectCache* cache = ExistingAXObjectCache()) {
      for (Node& node : NodeTraversal::DescendantsOf(*this)) {
        cache->Remove(&node);
      }
    }
  }

  GetStyleEngine().DidDetach();

  frame_->GetEventHandlerRegistry().DocumentDetached(*this);

  DocumentShutdownNotifier::NotifyContextDestroyed();
  SynchronousMutationNotifier::NotifyContextDestroyed();

  fetcher_->ClearContext();
  if (imports_controller_) {
    imports_controller_->Dispose();
    ClearImportsController();
  }

  if (media_query_matcher_)
    media_query_matcher_->DocumentDetached();

  lifecycle_.AdvanceTo(DocumentLifecycle::kStopped);
  CHECK(!View()->IsAttached());

  ExecutionContext::NotifyContextDestroyed();
  CHECK(!View()->IsAttached());

  needs_to_record_ukm_outlive_time_ = IsInMainFrame();
  if (needs_to_record_ukm_outlive_time_) {
    UkmRecorder();
  }

  frame_ = nullptr;

  document_outlive_time_reporter_ =
      std::make_unique<DocumentOutliveTimeReporter>(this);
}
