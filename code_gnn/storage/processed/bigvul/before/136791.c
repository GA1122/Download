void LocalDOMWindow::alert(ScriptState* script_state, const String& message) {
  if (!GetFrame())
    return;

  if (document()->IsSandboxed(kSandboxModals)) {
    UseCounter::Count(document(), WebFeature::kDialogInSandboxedContext);
    GetFrameConsole()->AddMessage(ConsoleMessage::Create(
        kSecurityMessageSource, kErrorMessageLevel,
        "Ignored call to 'alert()'. The document is sandboxed, and the "
        "'allow-modals' keyword is not set."));
    return;
  }

  switch (document()->GetEngagementLevel()) {
    case mojom::blink::EngagementLevel::NONE:
      UseCounter::Count(document(), WebFeature::kAlertEngagementNone);
      break;
    case mojom::blink::EngagementLevel::MINIMAL:
      UseCounter::Count(document(), WebFeature::kAlertEngagementMinimal);
      break;
    case mojom::blink::EngagementLevel::LOW:
      UseCounter::Count(document(), WebFeature::kAlertEngagementLow);
      break;
    case mojom::blink::EngagementLevel::MEDIUM:
      UseCounter::Count(document(), WebFeature::kAlertEngagementMedium);
      break;
    case mojom::blink::EngagementLevel::HIGH:
      UseCounter::Count(document(), WebFeature::kAlertEngagementHigh);
      break;
    case mojom::blink::EngagementLevel::MAX:
      UseCounter::Count(document(), WebFeature::kAlertEngagementMax);
      break;
  }

  if (v8::MicrotasksScope::IsRunningMicrotasks(script_state->GetIsolate())) {
    UseCounter::Count(document(), WebFeature::kDuring_Microtask_Alert);
  }

  document()->UpdateStyleAndLayoutTree();

  Page* page = GetFrame()->GetPage();
  if (!page)
    return;

  UseCounter::CountCrossOriginIframe(*document(),
                                     WebFeature::kCrossOriginWindowAlert);

  page->GetChromeClient().OpenJavaScriptAlert(GetFrame(), message);
}
