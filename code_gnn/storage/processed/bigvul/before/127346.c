 static const MediaQueryEvaluator& screenEval()
 {
     DEFINE_STATIC_LOCAL(const MediaQueryEvaluator, staticScreenEval, ("screen"));
    return staticScreenEval;
}
