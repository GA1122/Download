void StyleResolver::finishAppendAuthorStyleSheets()
{
    collectFeatures();

    if (document().renderer() && document().renderer()->style())
        document().renderer()->style()->font().update(fontSelector());

    collectViewportRules();
}
