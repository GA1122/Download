void Document::AddConsoleMessage(ConsoleMessage* console_message) {
  if (!IsContextThread()) {
    GetTaskRunner(TaskType::kUnthrottled)
        ->PostTask(BLINK_FROM_HERE,
                   CrossThreadBind(
                       &RunAddConsoleMessageTask, console_message->Source(),
                       console_message->Level(), console_message->Message(),
                       WrapCrossThreadPersistent(this)));
    return;
  }

  if (!frame_)
    return;

  if (console_message->Location()->IsUnknown()) {
    unsigned line_number = 0;
    if (!IsInDocumentWrite() && GetScriptableDocumentParser()) {
      ScriptableDocumentParser* parser = GetScriptableDocumentParser();
      if (parser->IsParsingAtLineNumber())
        line_number = parser->LineNumber().OneBasedInt();
    }
    Vector<DOMNodeId> nodes(console_message->Nodes());
    console_message = ConsoleMessage::Create(
        console_message->Source(), console_message->Level(),
        console_message->Message(),
        SourceLocation::Create(Url().GetString(), line_number, 0, nullptr));
    console_message->SetNodes(frame_, std::move(nodes));
  }
  frame_->Console().AddMessage(console_message);
}
