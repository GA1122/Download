    static bool linkMediaAttributeIsScreen(const String& attributeValue)
    {
        if (attributeValue.isEmpty())
            return true;
        RefPtr<MediaQuerySet> mediaQueries = MediaQuerySet::createAllowingDescriptionSyntax(attributeValue);
    
        MediaQueryEvaluator mediaQueryEvaluator("screen");
        return mediaQueryEvaluator.eval(mediaQueries.get());
    }
