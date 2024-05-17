void TabHelper::OnGetAppInstallState(content::RenderFrameHost* host,
                                     const GURL& requestor_url,
                                     int return_route_id,
                                     int callback_id) {
  ExtensionRegistry* registry =
      ExtensionRegistry::Get(web_contents()->GetBrowserContext());
  const ExtensionSet& extensions = registry->enabled_extensions();
  const ExtensionSet& disabled_extensions = registry->disabled_extensions();

  std::string state;
  if (extensions.GetHostedAppByURL(requestor_url))
    state = extension_misc::kAppStateInstalled;
  else if (disabled_extensions.GetHostedAppByURL(requestor_url))
    state = extension_misc::kAppStateDisabled;
  else
    state = extension_misc::kAppStateNotInstalled;

  host->Send(new ExtensionMsg_GetAppInstallStateResponse(return_route_id, state,
                                                         callback_id));
}
