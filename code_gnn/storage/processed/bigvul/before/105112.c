PassRefPtr<Range> Range::create(PassRefPtr<Document> ownerDocument)
{
    return adoptRef(new Range(ownerDocument));
}
