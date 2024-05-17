bool WasModuleLoadSuccessful(
    Resource* resource,
    HeapVector<Member<ConsoleMessage>>* error_messages) {

  DCHECK(error_messages);

  if (resource) {
    SubresourceIntegrityHelper::GetConsoleMessages(
        resource->IntegrityReportInfo(), error_messages);
  }

  if (!resource || resource->ErrorOccurred() ||
      resource->IntegrityDisposition() !=
          ResourceIntegrityDisposition::kPassed) {
    return false;
  }

  const auto& response = resource->GetResponse();
  if (response.IsHTTP() && !FetchUtils::IsOkStatus(response.HttpStatusCode())) {
    return false;
  }

  if (!MIMETypeRegistry::IsSupportedJavaScriptMIMEType(
          response.HttpContentType())) {
    String message =
        "Failed to load module script: The server responded with a "
        "non-JavaScript MIME type of \"" +
        response.HttpContentType() +
        "\". Strict MIME type checking is enforced for module scripts per "
        "HTML spec.";
    error_messages->push_back(ConsoleMessage::CreateForRequest(
        kJSMessageSource, kErrorMessageLevel, message,
        response.Url().GetString(), nullptr, resource->Identifier()));
    return false;
  }

  return true;
}
