EvalJsResult GetDocumentOrigin(FrameTreeNode* ftn) {
  return EvalJs(ftn, "document.origin;");
}
