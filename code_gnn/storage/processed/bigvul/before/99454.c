 bool NPJSObject::NP_Enumerate(NPObject* npObject, NPIdentifier** identifiers, uint32_t* identifierCount)
 {
     return toNPJSObject(npObject)->enumerate(identifiers, identifierCount);
}
