static NPObject *NPN_RetainObject(NPObject *npObject)
{
    retainNPObject(npObject);
    return npObject;
}
