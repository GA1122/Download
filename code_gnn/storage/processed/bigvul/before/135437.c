String Document::userAgent() const
{
    return frame() ? frame()->loader().userAgent() : String();
}
