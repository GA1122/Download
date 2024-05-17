void WebContentsImpl::SetImportance(ChildProcessImportance importance) {
  for (auto* host : GetAllRenderWidgetHosts())
    host->SetImportance(importance);

}
