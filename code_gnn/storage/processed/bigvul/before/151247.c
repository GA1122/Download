HeapVector<Member<Document>> InspectorPageAgent::ImportsForFrame(
    LocalFrame* frame) {
  HeapVector<Member<Document>> result;
  Document* root_document = frame->GetDocument();

  if (HTMLImportsController* controller = root_document->ImportsController()) {
    for (size_t i = 0; i < controller->LoaderCount(); ++i) {
      if (Document* document = controller->LoaderAt(i)->GetDocument())
        result.push_back(document);
    }
  }

  return result;
}
