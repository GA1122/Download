LocalDOMWindow::LocalDOMWindow(LocalFrame& frame)
    : DOMWindow(frame),
      visualViewport_(DOMVisualViewport::Create(this)),
      unused_preloads_timer_(frame.GetTaskRunner(TaskType::kUnspecedTimer),
                             this,
                             &LocalDOMWindow::WarnUnusedPreloads),
      should_print_when_finished_loading_(false) {}
