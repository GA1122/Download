void DOMWindow::close(LocalDOMWindow* incumbent_window) {
  if (!GetFrame() || !GetFrame()->IsMainFrame())
    return;

  Page* page = GetFrame()->GetPage();
  if (!page)
    return;

  Document* active_document = nullptr;
  if (incumbent_window) {
    DCHECK(IsMainThread());
    active_document = incumbent_window->document();
    if (!active_document)
      return;

    if (!active_document->GetFrame() ||
        !active_document->GetFrame()->CanNavigate(*GetFrame()))
      return;
  }

  Settings* settings = GetFrame()->GetSettings();
  bool allow_scripts_to_close_windows =
      settings && settings->GetAllowScriptsToCloseWindows();

  if (!page->OpenedByDOM() && GetFrame()->Client()->BackForwardLength() > 1 &&
      !allow_scripts_to_close_windows) {
    if (active_document) {
      active_document->domWindow()->GetFrameConsole()->AddMessage(
          ConsoleMessage::Create(
              kJSMessageSource, kWarningMessageLevel,
              "Scripts may close only the windows that were opened by it."));
    }
    return;
  }

  if (!GetFrame()->ShouldClose())
    return;

  ExecutionContext* execution_context = nullptr;
  if (IsLocalDOMWindow()) {
    execution_context = blink::ToLocalDOMWindow(this)->GetExecutionContext();
  }
  probe::breakableLocation(execution_context, "DOMWindow.close");

  page->CloseSoon();

  window_is_closing_ = true;
}
