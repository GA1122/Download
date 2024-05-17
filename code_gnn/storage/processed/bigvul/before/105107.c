short Range::compareBoundaryPoints(Node* containerA, int offsetA, Node* containerB, int offsetB, ExceptionCode& ec)
{
    ASSERT(containerA);
    ASSERT(containerB);

    if (!containerA)
        return -1;
    if (!containerB)
        return 1;


    if (containerA == containerB) {
        if (offsetA == offsetB)
            return 0;            
        if (offsetA < offsetB)
            return -1;           
        else
            return 1;            
    }

    Node* c = containerB;
    while (c && c->parentNode() != containerA)
        c = c->parentNode();
    if (c) {
        int offsetC = 0;
        Node* n = containerA->firstChild();
        while (n != c && offsetC < offsetA) {
            offsetC++;
            n = n->nextSibling();
        }

        if (offsetA <= offsetC)
            return -1;               
        else
            return 1;                
    }

    c = containerA;
    while (c && c->parentNode() != containerB)
        c = c->parentNode();
    if (c) {
        int offsetC = 0;
        Node* n = containerB->firstChild();
        while (n != c && offsetC < offsetB) {
            offsetC++;
            n = n->nextSibling();
        }

        if (offsetC < offsetB)
            return -1;               
        else
            return 1;                
    }

    Node* commonAncestor = commonAncestorContainer(containerA, containerB);
    if (!commonAncestor) {
        ec = WRONG_DOCUMENT_ERR;
        return 0;
    }
    Node* childA = containerA;
    while (childA && childA->parentNode() != commonAncestor)
        childA = childA->parentNode();
    if (!childA)
        childA = commonAncestor;
    Node* childB = containerB;
    while (childB && childB->parentNode() != commonAncestor)
        childB = childB->parentNode();
    if (!childB)
        childB = commonAncestor;

    if (childA == childB)
        return 0;  

    Node* n = commonAncestor->firstChild();
    while (n) {
        if (n == childA)
            return -1;  
        if (n == childB)
            return 1;  
        n = n->nextSibling();
    }

    ASSERT_NOT_REACHED();
    return 0;
}
