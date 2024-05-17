 void SerializerMarkupAccumulator::appendEndTag(Node* node)
{
    if (node->isElementNode() && !shouldIgnoreElement(toElement(node)))
         MarkupAccumulator::appendEndTag(node);
 }
