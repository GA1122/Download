String Document::compatMode() const
{
    return inQuirksMode() ? "BackCompat" : "CSS1Compat";
}
