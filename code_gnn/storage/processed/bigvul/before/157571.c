  void OnGetDocument(std::unique_ptr<dom::GetDocumentResult> result) {
    devtools_client_->GetDOM()->QuerySelector(
        dom::QuerySelectorParams::Builder()
            .SetNodeId(result->GetRoot()->GetNodeId())
            .SetSelector("<o_O>")
            .Build(),
        base::BindOnce(&HeadlessDevToolsMethodCallErrorTest::OnQuerySelector,
                       base::Unretained(this)));
  }
