PassRefPtr<Node> Range::processAncestorsAndTheirSiblings(ActionType action, Node* container, ContentsProcessDirection direction, PassRefPtr<Node> passedClonedContainer, Node* commonRoot, ExceptionCode& ec)
{
    typedef Vector<RefPtr<Node> > NodeVector;

    RefPtr<Node> clonedContainer = passedClonedContainer;
    Vector<RefPtr<Node> > ancestors;
    for (ContainerNode* n = container->parentNode(); n && n != commonRoot; n = n->parentNode())
        ancestors.append(n);

    RefPtr<Node> firstChildInAncestorToProcess = direction == ProcessContentsForward ? container->nextSibling() : container->previousSibling();
    for (Vector<RefPtr<Node> >::const_iterator it = ancestors.begin(); it != ancestors.end(); it++) {
        RefPtr<Node> ancestor = *it;
        if (action == EXTRACT_CONTENTS || action == CLONE_CONTENTS) {
            if (RefPtr<Node> clonedAncestor = ancestor->cloneNode(false)) {  
                clonedAncestor->appendChild(clonedContainer, ec);
                clonedContainer = clonedAncestor;
            }
        }

        ASSERT(!firstChildInAncestorToProcess || firstChildInAncestorToProcess->parentNode() == ancestor);
        
        NodeVector nodes;
        for (Node* child = firstChildInAncestorToProcess.get(); child;
            child = (direction == ProcessContentsForward) ? child->nextSibling() : child->previousSibling())
            nodes.append(child);

        for (NodeVector::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
            Node* child = it->get();
            switch (action) {
            case DELETE_CONTENTS:
                ancestor->removeChild(child, ec);
                break;
            case EXTRACT_CONTENTS:  
                if (direction == ProcessContentsForward)
                    clonedContainer->appendChild(child, ec);
                else
                    clonedContainer->insertBefore(child, clonedContainer->firstChild(), ec);
                break;
            case CLONE_CONTENTS:
                if (direction == ProcessContentsForward)
                    clonedContainer->appendChild(child->cloneNode(true), ec);
                else
                    clonedContainer->insertBefore(child->cloneNode(true), clonedContainer->firstChild(), ec);
                break;
            }
        }
        firstChildInAncestorToProcess = direction == ProcessContentsForward ? ancestor->nextSibling() : ancestor->previousSibling();
    }

    return clonedContainer.release();
}
