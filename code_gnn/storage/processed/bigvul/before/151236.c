void InspectorPageAgent::DidRunJavaScriptDialog() {
  GetFrontend()->flush();
}
