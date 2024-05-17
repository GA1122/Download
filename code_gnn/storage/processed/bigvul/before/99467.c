static Identifier identifierFromIdentifierRep(ExecState* exec, IdentifierRep* identifierRep)
{
    ASSERT(identifierRep->isString());

    const char* string = identifierRep->string();
    int length = strlen(string);

    return Identifier(exec, String::fromUTF8WithLatin1Fallback(string, length).impl());
}
