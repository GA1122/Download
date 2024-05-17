bool DOMPatchSupport::insertBeforeAndMarkAsUsed(ContainerNode* parentNode, Digest* digest, Node* anchor, ExceptionCode& ec)
{
    bool result = m_domEditor->insertBefore(parentNode, digest->m_node, anchor, ec);
    markNodeAsUsed(digest);
    return result;
}
