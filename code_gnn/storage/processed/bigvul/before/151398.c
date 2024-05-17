ResourceFetcher* FrameFetchContext::CreateFetcher(DocumentLoader* loader,
                                                  Document* document) {
  FrameFetchContext* context = new FrameFetchContext(loader, document);
  ResourceFetcher* fetcher = ResourceFetcher::Create(context);

  if (loader && context->GetSettings()->GetSavePreviousDocumentResources() !=
                    SavePreviousDocumentResources::kNever) {
    if (Document* previous_document = context->GetFrame()->GetDocument()) {
      if (previous_document->IsSecureTransitionTo(loader->Url())) {
        fetcher->HoldResourcesFromPreviousFetcher(
            previous_document->Loader()->Fetcher());
      }
    }
  }

  return fetcher;
}
