MediaList* CSSStyleSheet::media() {
  if (!media_queries_)
    media_queries_ = MediaQuerySet::Create();

  if (!media_cssom_wrapper_)
    media_cssom_wrapper_ = MediaList::Create(media_queries_.get(),
                                             const_cast<CSSStyleSheet*>(this));
  return media_cssom_wrapper_.Get();
}
