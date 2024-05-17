String Document::readyState() const
{
    DEFINE_STATIC_LOCAL(const String, loading, (ASCIILiteral("loading")));
    DEFINE_STATIC_LOCAL(const String, interactive, (ASCIILiteral("interactive")));
    DEFINE_STATIC_LOCAL(const String, complete, (ASCIILiteral("complete")));

    switch (m_readyState) {
    case Loading:
        return loading;
    case Interactive:
        return interactive;
    case Complete:
        return complete;
    }

    ASSERT_NOT_REACHED();
    return String();
}
