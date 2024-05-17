 void HTMLPreloadScanner::updatePredictedBaseElementURL(const KURL& baseElementURL)
{
    if (!m_predictedBaseElementURL.isEmpty())
        return;
    m_predictedBaseElementURL = baseElementURL;
}
