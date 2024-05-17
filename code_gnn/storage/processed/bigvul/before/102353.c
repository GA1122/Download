void ExtensionHelper::DidCreateDocumentElement(WebFrame* frame) {
  extension_dispatcher_->user_script_slave()->InjectScripts(
      frame, UserScript::DOCUMENT_START);
}
