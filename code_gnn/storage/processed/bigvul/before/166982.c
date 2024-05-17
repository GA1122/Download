void CSSStyleSheet::SetMediaQueries(
    scoped_refptr<MediaQuerySet> media_queries) {
  media_queries_ = std::move(media_queries);
  if (media_cssom_wrapper_ && media_queries_)
    media_cssom_wrapper_->Reattach(media_queries_.get());
}
