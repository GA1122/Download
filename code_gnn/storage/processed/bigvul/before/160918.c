void DOMWindow::postMessage(scoped_refptr<SerializedScriptValue> message,
                            const MessagePortArray& ports,
                            const String& target_origin,
                            LocalDOMWindow* source,
                            ExceptionState& exception_state) {
  if (!IsCurrentlyDisplayedInFrame())
    return;

  Document* source_document = source->document();

  scoped_refptr<const SecurityOrigin> target;
  if (target_origin == "/") {
    if (!source_document)
      return;
    target = source_document->GetSecurityOrigin();
  } else if (target_origin != "*") {
    target = SecurityOrigin::CreateFromString(target_origin);
    if (target->IsUnique()) {
      exception_state.ThrowDOMException(
          kSyntaxError, "Invalid target origin '" + target_origin +
                            "' in a call to 'postMessage'.");
      return;
    }
  }

  auto channels = MessagePort::DisentanglePorts(GetExecutionContext(), ports,
                                                exception_state);
  if (exception_state.HadException())
    return;

  if (!source_document)
    return;

  const SecurityOrigin* security_origin = source_document->GetSecurityOrigin();
  bool has_suborigin = source_document->GetSecurityOrigin()->HasSuborigin();
  Suborigin::SuboriginPolicyOptions unsafe_send_opt =
      Suborigin::SuboriginPolicyOptions::kUnsafePostMessageSend;

  String source_origin =
      (has_suborigin &&
       security_origin->GetSuborigin()->PolicyContains(unsafe_send_opt))
          ? security_origin->ToPhysicalOriginString()
          : security_origin->ToString();
  String source_suborigin =
      has_suborigin ? security_origin->GetSuborigin()->GetName() : String();

  KURL target_url = IsLocalDOMWindow()
                        ? blink::ToLocalDOMWindow(this)->document()->Url()
                        : KURL(NullURL(), GetFrame()
                                              ->GetSecurityContext()
                                              ->GetSecurityOrigin()
                                              ->ToString());
  if (MixedContentChecker::IsMixedContent(source_document->GetSecurityOrigin(),
                                          target_url)) {
    UseCounter::Count(source->GetFrame(),
                      WebFeature::kPostMessageFromSecureToInsecure);
  } else if (MixedContentChecker::IsMixedContent(
                 GetFrame()->GetSecurityContext()->GetSecurityOrigin(),
                 source_document->Url())) {
    UseCounter::Count(source->GetFrame(),
                      WebFeature::kPostMessageFromInsecureToSecure);
    if (MixedContentChecker::IsMixedContent(
            GetFrame()->Tree().Top().GetSecurityContext()->GetSecurityOrigin(),
            source_document->Url())) {
      UseCounter::Count(source->GetFrame(),
                        WebFeature::kPostMessageFromInsecureToSecureToplevel);
    }
  }

  if (!source_document->GetContentSecurityPolicy()->AllowConnectToSource(
          target_url, RedirectStatus::kNoRedirect,
          SecurityViolationReportingPolicy::kSuppressReporting)) {
    UseCounter::Count(
        source->GetFrame(),
        WebFeature::kPostMessageOutgoingWouldBeBlockedByConnectSrc);
  }

  MessageEvent* event =
      MessageEvent::Create(std::move(channels), std::move(message),
                           source_origin, String(), source, source_suborigin);

  SchedulePostMessage(event, std::move(target), source_document);
}
