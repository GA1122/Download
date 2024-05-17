void WebKitTestController::OnClearDevToolsLocalStorage() {
  ShellBrowserContext* browser_context =
      ShellContentBrowserClient::Get()->browser_context();
  StoragePartition* storage_partition =
      BrowserContext::GetStoragePartition(browser_context, NULL);
  storage_partition->GetDOMStorageContext()->DeleteLocalStorage(
      content::LayoutTestDevToolsFrontend::GetDevToolsPathAsURL("", "")
          .GetOrigin());
}
