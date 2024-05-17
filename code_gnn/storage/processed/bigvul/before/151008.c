void DevToolsUIBindings::InspectedURLChanged(const std::string& url) {
  content::NavigationController& controller = web_contents()->GetController();
  content::NavigationEntry* entry = controller.GetActiveEntry();
  web_contents()->UpdateTitleForEntry(
      entry, base::UTF8ToUTF16(base::StringPrintf(kTitleFormat, url.c_str())));
}
