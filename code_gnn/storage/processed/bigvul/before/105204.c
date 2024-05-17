static Node* highestAncestorToWrapMarkup(const Range* range, EAnnotateForInterchange shouldAnnotate)
{
    ExceptionCode ec;
    Node* commonAncestor = range->commonAncestorContainer(ec);
    ASSERT(commonAncestor);
    Node* specialCommonAncestor = 0;
    if (shouldAnnotate == AnnotateForInterchange) {
        specialCommonAncestor = ancestorToRetainStructureAndAppearance(commonAncestor);

        if (Node* highestMailBlockquote = highestEnclosingNodeOfType(firstPositionInOrBeforeNode(range->firstNode()), isMailBlockquote, CanCrossEditingBoundary))
            specialCommonAncestor = highestMailBlockquote;
    }

    Node* checkAncestor = specialCommonAncestor ? specialCommonAncestor : commonAncestor;
    if (checkAncestor->renderer()) {
        Node* newSpecialCommonAncestor = highestEnclosingNodeOfType(firstPositionInNode(checkAncestor), &isElementPresentational);
        if (newSpecialCommonAncestor)
            specialCommonAncestor = newSpecialCommonAncestor;
    }

    if (!specialCommonAncestor && isTabSpanTextNode(commonAncestor))
        specialCommonAncestor = commonAncestor->parentNode();
    if (!specialCommonAncestor && isTabSpanNode(commonAncestor))
        specialCommonAncestor = commonAncestor;

    if (Node *enclosingAnchor = enclosingNodeWithTag(firstPositionInNode(specialCommonAncestor ? specialCommonAncestor : commonAncestor), aTag))
        specialCommonAncestor = enclosingAnchor;

    return specialCommonAncestor;
}
