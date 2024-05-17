void ExtensionDevToolsInfoBar::InfoBarDismissed() {
  std::map<ExtensionDevToolsClientHost*, base::Closure> copy = callbacks_;
  for (const auto& pair : copy)
    pair.second.Run();
}
