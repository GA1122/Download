void InspectorPageAgent::WillRunJavaScriptDialog() {
  GetFrontend()->flush();
}
