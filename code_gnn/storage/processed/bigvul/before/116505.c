void ExtensionDevToolsClientHost::SendDetachedEvent() {
  Profile* profile =
      Profile::FromBrowserContext(web_contents_->GetBrowserContext());
  if (profile != NULL && profile->GetExtensionEventRouter()) {
    ListValue args;
    args.Append(CreateDebuggeeId(tab_id_));

    std::string json_args;
    base::JSONWriter::Write(&args, false, &json_args);

    profile->GetExtensionEventRouter()->DispatchEventToExtension(
        extension_id_, keys::kOnDetach, json_args, profile, GURL());
  }
}
