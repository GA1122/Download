String StyledMarkupAccumulator::renderedText(const Node* node, const Range* range)
{
    if (!node->isTextNode())
        return String();

    ExceptionCode ec;
    const Text* textNode = static_cast<const Text*>(node);
    unsigned startOffset = 0;
    unsigned endOffset = textNode->length();

    if (range && node == range->startContainer(ec))
        startOffset = range->startOffset(ec);
    if (range && node == range->endContainer(ec))
        endOffset = range->endOffset(ec);

    Position start = createLegacyEditingPosition(const_cast<Node*>(node), startOffset);
    Position end = createLegacyEditingPosition(const_cast<Node*>(node), endOffset);
    return plainText(Range::create(node->document(), start, end).get());
}
