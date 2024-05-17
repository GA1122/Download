void HTMLCanvasElement::DidMoveToNewDocument(Document& old_document) {
  ContextLifecycleObserver::SetContext(&GetDocument());
  PageVisibilityObserver::SetContext(GetDocument().GetPage());
  HTMLElement::DidMoveToNewDocument(old_document);
}
