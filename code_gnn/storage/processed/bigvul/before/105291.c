void DOMPatchSupport::markNodeAsUsed(Digest* digest)
{
    Deque<Digest*> queue;
    queue.append(digest);
    while (!queue.isEmpty()) {
        Digest* first = queue.takeFirst();
        m_unusedNodesMap.remove(first->m_sha1);
        for (size_t i = 0; i < first->m_children.size(); ++i)
            queue.append(first->m_children[i].get());
    }
}
