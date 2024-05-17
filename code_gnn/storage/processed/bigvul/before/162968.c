void ManifestManager::OnManifestFetchComplete(
    const GURL& document_url,
    const blink::WebURLResponse& response,
    const std::string& data) {
  fetcher_.reset();
  if (response.IsNull() && data.empty()) {
    manifest_debug_info_ = nullptr;
    ManifestUmaUtil::FetchFailed(ManifestUmaUtil::FETCH_UNSPECIFIED_REASON);
    ResolveCallbacks(ResolveStateFailure);
    return;
  }

  ManifestUmaUtil::FetchSucceeded();
  GURL response_url = response.Url();
  base::StringPiece data_piece(data);
  ManifestParser parser(data_piece, response_url, document_url);
  parser.Parse();

  manifest_debug_info_ = blink::mojom::ManifestDebugInfo::New();
  manifest_debug_info_->raw_manifest = data;
  parser.TakeErrors(&manifest_debug_info_->errors);

  for (const auto& error : manifest_debug_info_->errors) {
    blink::WebConsoleMessage message;
    message.level = error->critical ? blink::WebConsoleMessage::kLevelError
                                    : blink::WebConsoleMessage::kLevelWarning;
    message.text =
        blink::WebString::FromUTF8(GetMessagePrefix() + error->message);
    message.url =
        render_frame()->GetWebFrame()->GetDocument().ManifestURL().GetString();
    message.line_number = error->line;
    message.column_number = error->column;
    render_frame()->GetWebFrame()->AddMessageToConsole(message);
  }

  if (parser.failed()) {
    ResolveCallbacks(ResolveStateFailure);
    return;
  }

  manifest_url_ = response.Url();
  manifest_ = parser.manifest();
  ResolveCallbacks(ResolveStateSuccess);
}
