Location* Document::location() const
{
    if (!frame())
        return 0;

    return domWindow()->location();
}
