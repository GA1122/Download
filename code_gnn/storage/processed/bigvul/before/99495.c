static bool NPN_IdentifierIsString(NPIdentifier identifier)
{
    return static_cast<IdentifierRep*>(identifier)->isString();
}
