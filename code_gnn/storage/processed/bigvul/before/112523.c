String Document::referrer() const
{
    if (frame())
        return frame()->loader()->referrer();
    return String();
}
