TokenPreloadScannerCheckpoint TokenPreloadScanner::createCheckpoint()
{
    TokenPreloadScannerCheckpoint checkpoint = m_checkpoints.size();
    m_checkpoints.append(Checkpoint(m_predictedBaseElementURL, m_inStyle, m_inScript, m_isAppCacheEnabled, m_isCSPEnabled, m_templateCount));
    return checkpoint;
}
