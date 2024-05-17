static NPIdentifier NPN_GetStringIdentifier(const NPUTF8 *name)
{
    return static_cast<NPIdentifier>(IdentifierRep::get(name));
}
