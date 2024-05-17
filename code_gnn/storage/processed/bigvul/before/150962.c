 void DevToolsUIBindings::AddDevToolsExtensionsToClient() {
   const extensions::ExtensionRegistry* registry =
       extensions::ExtensionRegistry::Get(profile_->GetOriginalProfile());
  if (!registry)
    return;

  base::ListValue results;
  for (const scoped_refptr<const extensions::Extension>& extension :
       registry->enabled_extensions()) {
    if (extensions::chrome_manifest_urls::GetDevToolsPage(extension.get())
            .is_empty())
      continue;
    std::unique_ptr<base::DictionaryValue> extension_info(
        new base::DictionaryValue());
    extension_info->Set(
        "startPage",
        new base::StringValue(extensions::chrome_manifest_urls::GetDevToolsPage(
                                  extension.get()).spec()));
    extension_info->Set("name", new base::StringValue(extension->name()));
    extension_info->Set("exposeExperimentalAPIs",
                        new base::FundamentalValue(
                            extension->permissions_data()->HasAPIPermission(
                                extensions::APIPermission::kExperimental)));
    results.Append(std::move(extension_info));
  }
  if (!results.empty()) {
    content::ChildProcessSecurityPolicy::GetInstance()->GrantScheme(
        web_contents_->GetMainFrame()->GetProcess()->GetID(),
        extensions::kExtensionScheme);
  }

  CallClientFunction("DevToolsAPI.addExtensions",
                     &results, NULL, NULL);
}
