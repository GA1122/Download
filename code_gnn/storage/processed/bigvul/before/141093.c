CoreProbeSink* Document::GetProbeSink() {
  LocalFrame* frame = GetFrame();
  if (!frame && TemplateDocumentHost())
    frame = TemplateDocumentHost()->GetFrame();
  return probe::ToCoreProbeSink(frame);
}
