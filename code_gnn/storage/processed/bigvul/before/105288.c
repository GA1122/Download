bool DOMPatchSupport::innerPatchChildren(ContainerNode* parentNode, const Vector<OwnPtr<Digest> >& oldList, const Vector<OwnPtr<Digest> >& newList, ExceptionCode& ec)
{
    pair<ResultMap, ResultMap> resultMaps = diff(oldList, newList);
    ResultMap& oldMap = resultMaps.first;
    ResultMap& newMap = resultMaps.second;

    Digest* oldHead = 0;
    Digest* oldBody = 0;

    HashMap<Digest*, Digest*> merges;
    HashSet<size_t, WTF::IntHash<size_t>, WTF::UnsignedWithZeroKeyHashTraits<size_t> > usedNewOrdinals;
    for (size_t i = 0; i < oldList.size(); ++i) {
        if (oldMap[i].first) {
            if (!usedNewOrdinals.contains(oldMap[i].second)) {
                usedNewOrdinals.add(oldMap[i].second);
                continue;
            }
            oldMap[i].first = 0;
            oldMap[i].second = 0;
        }

        if (oldList[i]->m_node->hasTagName(headTag)) {
            oldHead = oldList[i].get();
            continue;
        }
        if (oldList[i]->m_node->hasTagName(bodyTag)) {
            oldBody = oldList[i].get();
            continue;
        }

        if (!m_unusedNodesMap.contains(oldList[i]->m_sha1) && (!i || oldMap[i - 1].first) && (i == oldMap.size() - 1 || oldMap[i + 1].first)) {
            size_t anchorCandidate = i ? oldMap[i - 1].second + 1 : 0;
            size_t anchorAfter = i == oldMap.size() - 1 ? anchorCandidate + 1 : oldMap[i + 1].second;
            if (anchorAfter - anchorCandidate == 1 && anchorCandidate < newList.size())
                merges.set(newList[anchorCandidate].get(), oldList[i].get());
            else {
                if (!removeChildAndMoveToNew(oldList[i].get(), ec))
                    return false;
            }
        } else {
            if (!removeChildAndMoveToNew(oldList[i].get(), ec))
                return false;
        }
    }

    HashSet<size_t, WTF::IntHash<size_t>, WTF::UnsignedWithZeroKeyHashTraits<size_t> >  usedOldOrdinals;
    for (size_t i = 0; i < newList.size(); ++i) {
        if (!newMap[i].first)
            continue;
        size_t oldOrdinal = newMap[i].second;
        if (usedOldOrdinals.contains(oldOrdinal)) {
            newMap[i].first = 0;
            newMap[i].second = 0;
            continue;
        }
        usedOldOrdinals.add(oldOrdinal);
        markNodeAsUsed(newMap[i].first);
    }

    if (oldHead || oldBody) {
        for (size_t i = 0; i < newList.size(); ++i) {
            if (oldHead && newList[i]->m_node->hasTagName(headTag))
                merges.set(newList[i].get(), oldHead);
            if (oldBody && newList[i]->m_node->hasTagName(bodyTag))
                merges.set(newList[i].get(), oldBody);
        }
    }

    for (HashMap<Digest*, Digest*>::iterator it = merges.begin(); it != merges.end(); ++it) {
        if (!innerPatchNode(it->second, it->first, ec))
            return false;
    }

    for (size_t i = 0; i < newMap.size(); ++i) {
        if (newMap[i].first || merges.contains(newList[i].get()))
            continue;
        if (!insertBeforeAndMarkAsUsed(parentNode, newList[i].get(), parentNode->childNode(i), ec))
            return false;
    }

    for (size_t i = 0; i < oldMap.size(); ++i) {
        if (!oldMap[i].first)
            continue;
        RefPtr<Node> node = oldMap[i].first->m_node;
        Node* anchorNode = parentNode->childNode(oldMap[i].second);
        if (node.get() == anchorNode)
            continue;
        if (node->hasTagName(bodyTag) || node->hasTagName(headTag))
            continue;  

        if (!m_domEditor->insertBefore(parentNode, node.release(), anchorNode, ec))
            return false;
    }
    return true;
}
