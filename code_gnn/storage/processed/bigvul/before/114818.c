xmlDeregisterNodeDefault(xmlDeregisterNodeFunc func)
{
    xmlDeregisterNodeFunc old = xmlDeregisterNodeDefaultValue;
    
    __xmlRegisterCallbacks = 1;
    xmlDeregisterNodeDefaultValue = func;
    return(old);
}
