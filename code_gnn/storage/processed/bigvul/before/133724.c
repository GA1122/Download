static bool isTerminatingCharacter(UChar c)
{
    return (c == '&' || c == '/' || c == '"' || c == '\'' || c == '<' || c == '>' || c == ',');
}
