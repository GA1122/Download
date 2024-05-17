void HeadlessDevToolsManagerDelegate::PrintToPDF(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params,
    const CommandCallback& callback) {
  DCHECK(callback);

#if BUILDFLAG(ENABLE_BASIC_PRINTING)
  content::WebContents* web_contents = agent_host->GetWebContents();
  if (!web_contents) {
    callback.Run(CreateErrorResponse(command_id, kErrorServerError,
                                     "Command not supported on this endpoint"));
    return;
  }
  content::RenderFrameHost* rfh = web_contents->GetMainFrame();

  HeadlessPrintSettings settings;
  std::unique_ptr<base::DictionaryValue> response =
      ParsePrintSettings(command_id, params, &settings);
  if (response) {
    callback.Run(std::move(response));
    return;
  }
  HeadlessPrintManager::FromWebContents(web_contents)
      ->GetPDFContents(rfh, settings,
                       base::Bind(&PDFCreated, callback, command_id));
#else
  callback.Run(CreateErrorResponse(command_id, kErrorServerError,
                                   "Printing is not enabled"));
#endif
}
