void BrowserRenderProcessHost::InitExtensions() {
  std::vector<std::string> function_names;
  ExtensionFunctionDispatcher::GetAllFunctionNames(&function_names);
  Send(new ViewMsg_Extension_SetFunctionNames(function_names));
}
