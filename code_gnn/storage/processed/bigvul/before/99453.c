void NPJSObject::NP_Deallocate(NPObject* npObject)
{
    NPJSObject* npJSObject = toNPJSObject(npObject);
    delete npJSObject;
}
