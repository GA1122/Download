static bool mediaAttributeMatches(const MediaValuesCached& mediaValues, const String& attributeValue)
{
    MediaQuerySet* mediaQueries = MediaQuerySet::createOffMainThread(attributeValue);
    MediaQueryEvaluator mediaQueryEvaluator(mediaValues);
    return mediaQueryEvaluator.eval(mediaQueries);
}
