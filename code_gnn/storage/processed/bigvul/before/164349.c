ExtensionFunction::ResponseAction TabsGetCurrentFunction::Run() {
  DCHECK(dispatcher());

  WebContents* caller_contents = GetSenderWebContents();
  std::unique_ptr<base::ListValue> results;
  if (caller_contents && ExtensionTabUtil::GetTabId(caller_contents) >= 0) {
    results = tabs::Get::Results::Create(*ExtensionTabUtil::CreateTabObject(
        caller_contents, ExtensionTabUtil::kScrubTab, extension()));
  }
  return RespondNow(results ? ArgumentList(std::move(results)) : NoArguments());
}
