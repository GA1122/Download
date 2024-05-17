HTMLImportChild* HTMLImportsController::Load(const Document& parent_document,
                                             HTMLImportChildClient* client,
                                             FetchParameters& params) {
  DCHECK(client);

  HTMLImportLoader* parent_loader = LoaderFor(parent_document);
  HTMLImport* parent =
      parent_loader ? static_cast<HTMLImport*>(parent_loader->FirstImport())
                    : static_cast<HTMLImport*>(root_);

  const KURL& url = params.Url();

  DCHECK(!url.IsEmpty());
  DCHECK(url.IsValid());

  if (HTMLImportChild* child_to_share_with = root_->Find(url)) {
    HTMLImportLoader* loader = child_to_share_with->Loader();
    DCHECK(loader);
    HTMLImportChild* child = CreateChild(url, loader, parent, client);
    child->DidShareLoader();
    return child;
  }

  params.SetCrossOriginAccessControl(Master()->GetSecurityOrigin(),
                                     kCrossOriginAttributeAnonymous);

  HTMLImportLoader* loader = HTMLImportLoader::Create(this);
  ResourceFetcher* fetcher = parent->GetDocument()->Fetcher();
  RawResource::FetchImport(params, fetcher, loader);
  loaders_.push_back(loader);
  HTMLImportChild* child = CreateChild(url, loader, parent, client);
  child->DidStartLoading();
  return child;
}
