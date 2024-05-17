void ScriptLoader::childrenChanged()
{
    if (!m_parserInserted && m_element->inDocument())
        prepareScript();  
}
