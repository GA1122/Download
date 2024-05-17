HTMLImportChild* HTMLImportsController::CreateChild(
    const KURL& url,
    HTMLImportLoader* loader,
    HTMLImport* parent,
    HTMLImportChildClient* client) {
  HTMLImport::SyncMode mode = client->IsSync() && !MakesCycle(parent, url)
                                  ? HTMLImport::kSync
                                  : HTMLImport::kAsync;
  if (mode == HTMLImport::kAsync) {
    UseCounter::Count(root_->GetDocument(),
                      WebFeature::kHTMLImportsAsyncAttribute);
  }

  HTMLImportChild* child = new HTMLImportChild(url, loader, client, mode);
  parent->AppendImport(child);
  loader->AddImport(child);
  return root_->Add(child);
}
