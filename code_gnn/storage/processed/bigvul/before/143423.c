void LogGatedEvaluation(DocumentWriteGatedEvaluation reason)
{
    DEFINE_STATIC_LOCAL(EnumerationHistogram, gatedEvaluationHistogram, ("PreloadScanner.DocumentWrite.GatedEvaluation", GatedEvaluationLastValue));
    gatedEvaluationHistogram.count(reason);
}
