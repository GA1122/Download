Response InspectorPageAgent::setDocumentContent(const String& frame_id,
                                                const String& html) {
  LocalFrame* frame =
      IdentifiersFactory::FrameById(inspected_frames_, frame_id);
  if (!frame)
    return Response::Error("No frame for given id found");

  Document* document = frame->GetDocument();
  if (!document)
    return Response::Error("No Document instance to set HTML for");
  document->SetContent(html);
  return Response::OK();
}
