xmlRegisterNodeDefault(xmlRegisterNodeFunc func)
{
    xmlRegisterNodeFunc old = xmlRegisterNodeDefaultValue;
    
    __xmlRegisterCallbacks = 1;
    xmlRegisterNodeDefaultValue = func;
    return(old);
}
