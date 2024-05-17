PassRefPtr<DocumentFragment> Range::processContents(ActionType action, ExceptionCode& ec)
{
    typedef Vector<RefPtr<Node> > NodeVector;

    RefPtr<DocumentFragment> fragment;
    if (action == EXTRACT_CONTENTS || action == CLONE_CONTENTS)
        fragment = DocumentFragment::create(m_ownerDocument.get());

    ec = 0;
    if (collapsed(ec))
        return fragment.release();
    if (ec)
        return 0;

    RefPtr<Node> commonRoot = commonAncestorContainer(ec);
    if (ec)
        return 0;
    ASSERT(commonRoot);

    if (m_start.container() == m_end.container()) {
        processContentsBetweenOffsets(action, fragment, m_start.container(), m_start.offset(), m_end.offset(), ec);
        return fragment;
    }

    RefPtr<Node> partialStart = highestAncestorUnderCommonRoot(m_start.container(), commonRoot.get());
    RefPtr<Node> partialEnd = highestAncestorUnderCommonRoot(m_end.container(), commonRoot.get());



    RefPtr<Node> leftContents;
    if (m_start.container() != commonRoot && commonRoot->contains(m_start.container())) {
        leftContents = processContentsBetweenOffsets(action, 0, m_start.container(), m_start.offset(), lengthOfContentsInNode(m_start.container()), ec);
        leftContents = processAncestorsAndTheirSiblings(action, m_start.container(), ProcessContentsForward, leftContents, commonRoot.get(), ec);
    }

    RefPtr<Node> rightContents;
    if (m_end.container() != commonRoot && commonRoot->contains(m_end.container())) {
        rightContents = processContentsBetweenOffsets(action, 0, m_end.container(), 0, m_end.offset(), ec);
        rightContents = processAncestorsAndTheirSiblings(action, m_end.container(), ProcessContentsBackward, rightContents, commonRoot.get(), ec);
    }

    RefPtr<Node> processStart = childOfCommonRootBeforeOffset(m_start.container(), m_start.offset(), commonRoot.get());
    if (processStart && m_start.container() != commonRoot)  
        processStart = processStart->nextSibling();
    RefPtr<Node> processEnd = childOfCommonRootBeforeOffset(m_end.container(), m_end.offset(), commonRoot.get());

    if (action == EXTRACT_CONTENTS || action == DELETE_CONTENTS) {
        if (partialStart && commonRoot->contains(partialStart.get()))
            setStart(partialStart->parentNode(), partialStart->nodeIndex() + 1, ec);
        else if (partialEnd && commonRoot->contains(partialEnd.get()))
            setStart(partialEnd->parentNode(), partialEnd->nodeIndex(), ec);
        if (ec)
            return 0;
        m_end = m_start;
    }


    if ((action == EXTRACT_CONTENTS || action == CLONE_CONTENTS) && leftContents)
        fragment->appendChild(leftContents, ec);

    if (processStart) {
        NodeVector nodes;
        for (Node* n = processStart.get(); n && n != processEnd; n = n->nextSibling())
            nodes.append(n);
        processNodes(action, nodes, commonRoot, fragment, ec);
    }

    if ((action == EXTRACT_CONTENTS || action == CLONE_CONTENTS) && rightContents)
        fragment->appendChild(rightContents, ec);

    return fragment.release();
}
