void TokenPreloadScanner::rewindTo(TokenPreloadScannerCheckpoint checkpointIndex)
{
    ASSERT(checkpointIndex < m_checkpoints.size());  
    const Checkpoint& checkpoint = m_checkpoints[checkpointIndex];
    m_predictedBaseElementURL = checkpoint.predictedBaseElementURL;
    m_inStyle = checkpoint.inStyle;
    m_isAppCacheEnabled = checkpoint.isAppCacheEnabled;
    m_isCSPEnabled = checkpoint.isCSPEnabled;
    m_templateCount = checkpoint.templateCount;

    m_didRewind = true;
    m_inScript = checkpoint.inScript;

    m_cssScanner.reset();
    m_checkpoints.clear();
}
