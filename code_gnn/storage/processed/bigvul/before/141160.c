void Document::MediaQueryAffectingValueChanged() {
  GetStyleEngine().MediaQueryAffectingValueChanged();
  if (NeedsLayoutTreeUpdate())
    evaluate_media_queries_on_style_recalc_ = true;
  else
    EvaluateMediaQueryList();
  probe::MediaQueryResultChanged(this);
}
