bool CSSStyleSheet::MatchesMediaQueries(const MediaQueryEvaluator& evaluator) {
  viewport_dependent_media_query_results_.clear();
  device_dependent_media_query_results_.clear();

  if (!media_queries_)
    return true;
  return evaluator.Eval(*media_queries_,
                        &viewport_dependent_media_query_results_,
                        &device_dependent_media_query_results_);
}
