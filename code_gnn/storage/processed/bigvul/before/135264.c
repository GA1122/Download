HTMLHeadElement* Document::head() const
{
    Node* de = documentElement();
    if (!de)
        return 0;

    return Traversal<HTMLHeadElement>::firstChild(*de);
}
