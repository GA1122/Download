void ContainerNode::cloneChildNodes(ContainerNode *clone)
{
    TrackExceptionState es;
    for (Node* n = firstChild(); n && !es.hadException(); n = n->nextSibling())
        clone->appendChild(n->cloneNode(true), es);
}
