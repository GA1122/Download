void ContainerNode::cloneChildNodes(ContainerNode *clone)
{
    TrackExceptionState exceptionState;
    for (Node* n = firstChild(); n && !exceptionState.hadException(); n = n->nextSibling())
        clone->appendChild(n->cloneNode(true), exceptionState);
}
