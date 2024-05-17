bool HTMLImportsController::ShouldBlockScriptExecution(
    const Document& document) const {
  DCHECK_EQ(document.ImportsController(), this);
  if (HTMLImportLoader* loader = LoaderFor(document))
    return loader->ShouldBlockScriptExecution();
  return root_->GetState().ShouldBlockScriptExecution();
}
