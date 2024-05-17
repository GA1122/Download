void LocalDOMWindow::DispatchMessageEventWithOriginCheck(
    const SecurityOrigin* intended_target_origin,
    Event* event,
    std::unique_ptr<SourceLocation> location) {
  if (intended_target_origin) {
    const SecurityOrigin* security_origin = document()->GetSecurityOrigin();
    bool valid_target =
        intended_target_origin->IsSameSchemeHostPort(security_origin);

    if (!valid_target) {
      String message = ExceptionMessages::FailedToExecute(
          "postMessage", "DOMWindow",
          "The target origin provided ('" + intended_target_origin->ToString() +
              "') does not match the recipient window's origin ('" +
              document()->GetSecurityOrigin()->ToString() + "').");
      ConsoleMessage* console_message =
          ConsoleMessage::Create(kSecurityMessageSource, kErrorMessageLevel,
                                 message, std::move(location));
      GetFrameConsole()->AddMessage(console_message);
      return;
    }
  }

  KURL sender(static_cast<MessageEvent*>(event)->origin());
  if (!document()->GetContentSecurityPolicy()->AllowConnectToSource(
          sender, RedirectStatus::kNoRedirect,
          SecurityViolationReportingPolicy::kSuppressReporting)) {
    UseCounter::Count(
        GetFrame(), WebFeature::kPostMessageIncomingWouldBeBlockedByConnectSrc);
  }

  DispatchEvent(event);
}
