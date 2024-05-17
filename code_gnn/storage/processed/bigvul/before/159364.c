int GetCountFromBackgroundPage(const Extension* extension,
                               content::BrowserContext* context,
                               const std::string& variable_name) {
  ExtensionHost* host =
      ProcessManager::Get(context)->GetBackgroundHostForExtension(
          extension->id());
  if (!host || !host->host_contents())
    return -1;

  int count = -1;
  if (!ExecuteScriptAndExtractInt(
          host->host_contents(),
          "window.domAutomationController.send(" + variable_name + ")", &count))
    return -1;
  return count;
}
