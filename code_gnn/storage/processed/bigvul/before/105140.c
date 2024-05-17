void Range::processNodes(ActionType action, Vector<RefPtr<Node> >& nodes, PassRefPtr<Node> oldContainer, PassRefPtr<Node> newContainer, ExceptionCode& ec)
{
    for (unsigned i = 0; i < nodes.size(); i++) {
        switch (action) {
        case DELETE_CONTENTS:
            oldContainer->removeChild(nodes[i].get(), ec);
            break;
        case EXTRACT_CONTENTS:
            newContainer->appendChild(nodes[i].release(), ec);  
            break;
        case CLONE_CONTENTS:
            newContainer->appendChild(nodes[i]->cloneNode(true), ec);
            break;
        }
    }
}
