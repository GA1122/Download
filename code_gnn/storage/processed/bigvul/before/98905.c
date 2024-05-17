HTMLConstructionSite::HTMLConstructionSite(Document* document, FragmentScriptingPermission scriptingPermission, bool isParsingFragment)
    : m_document(document)
    , m_fragmentScriptingPermission(scriptingPermission)
    , m_isParsingFragment(isParsingFragment)
    , m_redirectAttachToFosterParent(false)
{
}
