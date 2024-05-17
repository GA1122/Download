static bool isCommandLineAPIGetter(const String16& name)
{
    if (name.length() != 2)
        return false;
    return name[0] == '$' && ((name[1] >= '0' && name[1] <= '4') || name[1] == '_');
}
