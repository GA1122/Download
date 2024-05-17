std::unique_ptr<base::DictionaryValue> DevToolsHttpHandler::SerializeDescriptor(
    scoped_refptr<DevToolsAgentHost> agent_host,
    const std::string& host) {
  std::unique_ptr<base::DictionaryValue> dictionary(new base::DictionaryValue);
  std::string id = agent_host->GetId();
  dictionary->SetString(kTargetIdField, id);
  std::string parent_id = agent_host->GetParentId();
  if (!parent_id.empty())
    dictionary->SetString(kTargetParentIdField, parent_id);
  dictionary->SetString(kTargetTypeField, agent_host->GetType());
  dictionary->SetString(kTargetTitleField,
                        net::EscapeForHTML(agent_host->GetTitle()));
  dictionary->SetString(kTargetDescriptionField, agent_host->GetDescription());

  GURL url = agent_host->GetURL();
  dictionary->SetString(kTargetUrlField, url.spec());

  GURL favicon_url = agent_host->GetFaviconURL();
  if (favicon_url.is_valid())
    dictionary->SetString(kTargetFaviconUrlField, favicon_url.spec());

  dictionary->SetString(kTargetWebSocketDebuggerUrlField,
                        base::StringPrintf("ws://%s%s%s", host.c_str(),
                                           kPageUrlPrefix, id.c_str()));
  std::string devtools_frontend_url =
      GetFrontendURLInternal(agent_host, id, host);
  dictionary->SetString(kTargetDevtoolsFrontendUrlField, devtools_frontend_url);

  return dictionary;
}
