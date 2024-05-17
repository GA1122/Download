FrameFetchContext::FrameFetchContext(DocumentLoader* loader, Document* document)
    : document_loader_(loader), document_(document) {
  DCHECK(GetFrame());
}
