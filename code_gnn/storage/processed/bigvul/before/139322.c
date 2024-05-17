static inline bool resolveCandidateRuns(Vector<CandidateRun>& runs)
{
    UScriptCode scriptExtensions[USCRIPT_CODE_LIMIT];
    UErrorCode errorCode = U_ZERO_ERROR;
    size_t length = runs.size();
    size_t nextResolvedRun = 0;
    for (size_t i = 0; i < length; i++) {
        CandidateRun& run = runs[i];
        nextResolvedRun = 0;

        if (run.script == USCRIPT_INHERITED)
            run.script = i > 0 ? runs[i - 1].script : USCRIPT_COMMON;

        int extensionsLength = uscript_getScriptExtensions(run.character,
            scriptExtensions, sizeof(scriptExtensions) / sizeof(scriptExtensions[0]),
            &errorCode);
        if (U_FAILURE(errorCode))
            return false;

        resolveRunBasedOnScriptExtensions(runs, run, i, length,
            scriptExtensions, extensionsLength, nextResolvedRun);
        resolveRunBasedOnScriptValue(runs, run, i, length,
            nextResolvedRun);
        for (size_t j = i; j < nextResolvedRun; j++)
            runs[j].script = runs[nextResolvedRun].script;

        i = std::max(i, nextResolvedRun);
    }
    return true;
}
