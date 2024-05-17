bool LocalDOMWindow::confirm(ScriptState* script_state, const String& message) {
  if (!GetFrame())
    return false;

  if (document()->IsSandboxed(kSandboxModals)) {
    UseCounter::Count(document(), WebFeature::kDialogInSandboxedContext);
    GetFrameConsole()->AddMessage(ConsoleMessage::Create(
        kSecurityMessageSource, kErrorMessageLevel,
        "Ignored call to 'confirm()'. The document is sandboxed, and the "
        "'allow-modals' keyword is not set."));
    return false;
  }

  switch (document()->GetEngagementLevel()) {
    case mojom::blink::EngagementLevel::NONE:
      UseCounter::Count(document(), WebFeature::kConfirmEngagementNone);
      break;
    case mojom::blink::EngagementLevel::MINIMAL:
      UseCounter::Count(document(), WebFeature::kConfirmEngagementMinimal);
      break;
    case mojom::blink::EngagementLevel::LOW:
      UseCounter::Count(document(), WebFeature::kConfirmEngagementLow);
      break;
    case mojom::blink::EngagementLevel::MEDIUM:
      UseCounter::Count(document(), WebFeature::kConfirmEngagementMedium);
      break;
    case mojom::blink::EngagementLevel::HIGH:
      UseCounter::Count(document(), WebFeature::kConfirmEngagementHigh);
      break;
    case mojom::blink::EngagementLevel::MAX:
      UseCounter::Count(document(), WebFeature::kConfirmEngagementMax);
      break;
  }

  if (v8::MicrotasksScope::IsRunningMicrotasks(script_state->GetIsolate())) {
    UseCounter::Count(document(), WebFeature::kDuring_Microtask_Confirm);
  }

  document()->UpdateStyleAndLayoutTree();

  Page* page = GetFrame()->GetPage();
  if (!page)
    return false;

  UseCounter::CountCrossOriginIframe(*document(),
                                     WebFeature::kCrossOriginWindowConfirm);

  return page->GetChromeClient().OpenJavaScriptConfirm(GetFrame(), message);
}
