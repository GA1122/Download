void TabsUpdateFunction::PopulateResult() {
  if (!has_callback())
    return;

  results_ = tabs::Get::Results::Create(*ExtensionTabUtil::CreateTabObject(
      web_contents_, ExtensionTabUtil::kScrubTab, extension()));
}
