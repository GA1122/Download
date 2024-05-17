static bool NPN_Enumerate(NPP, NPObject* npObject, NPIdentifier** identifiers, uint32_t* identifierCount)
{
    if (NP_CLASS_STRUCT_VERSION_HAS_ENUM(npObject->_class) && npObject->_class->enumerate)
        return npObject->_class->enumerate(npObject, identifiers, identifierCount);

    return false;
}
