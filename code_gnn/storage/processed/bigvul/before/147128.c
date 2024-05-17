bool DocumentLoader::MaybeLoadEmpty() {
  bool should_load_empty = !substitute_data_.IsValid() &&
                           (request_.Url().IsEmpty() ||
                            SchemeRegistry::ShouldLoadURLSchemeAsEmptyDocument(
                                request_.Url().Protocol()));
  if (!should_load_empty)
    return false;

  if (request_.Url().IsEmpty() &&
      !GetFrameLoader().StateMachine()->CreatingInitialEmptyDocument())
    request_.SetURL(BlankURL());
  response_ = ResourceResponse(request_.Url(), "text/html", 0, g_null_atom);
  FinishedLoading(MonotonicallyIncreasingTime());
  return true;
}
