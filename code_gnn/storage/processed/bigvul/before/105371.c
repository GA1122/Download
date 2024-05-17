static Node* getFocusedNode(Frame* frame)
{
    if (Document* doc = frame->document())
        return doc->focusedNode();
    return 0;
}
