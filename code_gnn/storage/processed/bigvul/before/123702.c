void ScriptLoader::didNotifySubtreeInsertionsToDocument()
{
    if (!m_parserInserted)
        prepareScript();  
}
