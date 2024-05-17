static HeapVector<Member<Resource>> CachedResourcesForFrame(LocalFrame* frame,
                                                            bool skip_xhrs) {
  HeapVector<Member<Resource>> result;
  Document* root_document = frame->GetDocument();
  HeapVector<Member<Document>> loaders =
      InspectorPageAgent::ImportsForFrame(frame);

  CachedResourcesForDocument(root_document, result, skip_xhrs);
  for (size_t i = 0; i < loaders.size(); ++i)
    CachedResourcesForDocument(loaders[i], result, skip_xhrs);

  return result;
}
