String LocalDOMWindow::prompt(ScriptState* script_state,
                              const String& message,
                              const String& default_value) {
  if (!GetFrame())
    return String();

  if (document()->IsSandboxed(kSandboxModals)) {
    UseCounter::Count(document(), WebFeature::kDialogInSandboxedContext);
    GetFrameConsole()->AddMessage(ConsoleMessage::Create(
        kSecurityMessageSource, kErrorMessageLevel,
        "Ignored call to 'prompt()'. The document is sandboxed, and the "
        "'allow-modals' keyword is not set."));
    return String();
  }

  switch (document()->GetEngagementLevel()) {
    case mojom::blink::EngagementLevel::NONE:
      UseCounter::Count(document(), WebFeature::kPromptEngagementNone);
      break;
    case mojom::blink::EngagementLevel::MINIMAL:
      UseCounter::Count(document(), WebFeature::kPromptEngagementMinimal);
      break;
    case mojom::blink::EngagementLevel::LOW:
      UseCounter::Count(document(), WebFeature::kPromptEngagementLow);
      break;
    case mojom::blink::EngagementLevel::MEDIUM:
      UseCounter::Count(document(), WebFeature::kPromptEngagementMedium);
      break;
    case mojom::blink::EngagementLevel::HIGH:
      UseCounter::Count(document(), WebFeature::kPromptEngagementHigh);
      break;
    case mojom::blink::EngagementLevel::MAX:
      UseCounter::Count(document(), WebFeature::kPromptEngagementMax);
      break;
  }

  if (v8::MicrotasksScope::IsRunningMicrotasks(script_state->GetIsolate())) {
    UseCounter::Count(document(), WebFeature::kDuring_Microtask_Prompt);
  }

  document()->UpdateStyleAndLayoutTree();

  Page* page = GetFrame()->GetPage();
  if (!page)
    return String();

  String return_value;
  if (page->GetChromeClient().OpenJavaScriptPrompt(GetFrame(), message,
                                                   default_value, return_value))
    return return_value;

  UseCounter::CountCrossOriginIframe(*document(),
                                     WebFeature::kCrossOriginWindowPrompt);

  return String();
}
