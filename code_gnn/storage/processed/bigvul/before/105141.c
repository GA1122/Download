PassRefPtr<Range> rangeOfContents(Node* node)
{
    ASSERT(node);
    RefPtr<Range> range = Range::create(node->document());
    int exception = 0;
    range->selectNodeContents(node, exception);
    return range.release();
}
