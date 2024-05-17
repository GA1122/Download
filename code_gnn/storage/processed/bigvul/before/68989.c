static const char* FLTGetPropertyName(CPLXMLNode* psXMLNode)
{
    const char* pszPropertyName;

    pszPropertyName = CPLGetXMLValue(psXMLNode, "PropertyName", NULL);
    if( pszPropertyName == NULL )  
        pszPropertyName = CPLGetXMLValue(psXMLNode, "ValueReference", NULL);
    return pszPropertyName;
}
