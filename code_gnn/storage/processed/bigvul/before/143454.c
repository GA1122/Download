bool TokenPreloadScanner::shouldEvaluateForDocumentWrite(const String& source)
{
    const int kMaxLengthForEvaluating = 1024;
    if (!m_documentParameters->doDocumentWritePreloadScanning)
        return false;

    DEFINE_STATIC_LOCAL(CustomCountHistogram, scriptLengthHistogram, ("PreloadScanner.DocumentWrite.ScriptLength", 0, 50000, 50));
    scriptLengthHistogram.count(source.length());

    if (source.length() > kMaxLengthForEvaluating) {
        LogGatedEvaluation(GatedEvaluationScriptTooLong);
        return false;
    }
    if (source.find("document.write") == WTF::kNotFound
        || source.findIgnoringASCIICase("src") == WTF::kNotFound) {
        LogGatedEvaluation(GatedEvaluationNoLikelyScript);
        return false;
    }
    if (source.findIgnoringASCIICase("<sc") == WTF::kNotFound
        && source.findIgnoringASCIICase("%3Csc") == WTF::kNotFound) {
        LogGatedEvaluation(GatedEvaluationNoLikelyScript);
        return false;
    }
    if (source.find("while") != WTF::kNotFound
        || source.find("for(") != WTF::kNotFound
        || source.find("for ") != WTF::kNotFound) {
        LogGatedEvaluation(GatedEvaluationLooping);
        return false;
    }
    if (source.find("jQuery") != WTF::kNotFound
        || source.find("$.") != WTF::kNotFound
        || source.find("$(") != WTF::kNotFound) {
        LogGatedEvaluation(GatedEvaluationPopularLibrary);
        return false;
    }
    if (source.find("Math.random") != WTF::kNotFound
        || source.find("Date") != WTF::kNotFound) {
        LogGatedEvaluation(GatedEvaluationNondeterminism);
        return false;
    }
    return true;
}
