DOMPatchSupport::diff(const Vector<OwnPtr<Digest> >& oldList, const Vector<OwnPtr<Digest> >& newList)
{
    ResultMap newMap(newList.size());
    ResultMap oldMap(oldList.size());

    for (size_t i = 0; i < oldMap.size(); ++i) {
        oldMap[i].first = 0;
        oldMap[i].second = 0;
    }

    for (size_t i = 0; i < newMap.size(); ++i) {
        newMap[i].first = 0;
        newMap[i].second = 0;
    }

    for (size_t i = 0; i < oldList.size() && i < newList.size() && oldList[i]->m_sha1 == newList[i]->m_sha1; ++i) {
        oldMap[i].first = oldList[i].get();
        oldMap[i].second = i;
        newMap[i].first = newList[i].get();
        newMap[i].second = i;
    }
    for (size_t i = 0; i < oldList.size() && i < newList.size() && oldList[oldList.size() - i - 1]->m_sha1 == newList[newList.size() - i - 1]->m_sha1; ++i) {
        size_t oldIndex = oldList.size() - i - 1;
        size_t newIndex = newList.size() - i - 1;
        oldMap[oldIndex].first = oldList[oldIndex].get();
        oldMap[oldIndex].second = newIndex;
        newMap[newIndex].first = newList[newIndex].get();
        newMap[newIndex].second = oldIndex;
    }

    typedef HashMap<String, Vector<size_t> > DiffTable;
    DiffTable newTable;
    DiffTable oldTable;

    for (size_t i = 0; i < newList.size(); ++i) {
        DiffTable::iterator it = newTable.add(newList[i]->m_sha1, Vector<size_t>()).iterator;
        it->second.append(i);
    }

    for (size_t i = 0; i < oldList.size(); ++i) {
        DiffTable::iterator it = oldTable.add(oldList[i]->m_sha1, Vector<size_t>()).iterator;
        it->second.append(i);
    }

    for (DiffTable::iterator newIt = newTable.begin(); newIt != newTable.end(); ++newIt) {
        if (newIt->second.size() != 1)
            continue;

        DiffTable::iterator oldIt = oldTable.find(newIt->first);
        if (oldIt == oldTable.end() || oldIt->second.size() != 1)
            continue;

        newMap[newIt->second[0]] = make_pair(newList[newIt->second[0]].get(), oldIt->second[0]);
        oldMap[oldIt->second[0]] = make_pair(oldList[oldIt->second[0]].get(), newIt->second[0]);
    }

    for (size_t i = 0; newList.size() > 0 && i < newList.size() - 1; ++i) {
        if (!newMap[i].first || newMap[i + 1].first)
            continue;

        size_t j = newMap[i].second + 1;
        if (j < oldMap.size() && !oldMap[j].first && newList[i + 1]->m_sha1 == oldList[j]->m_sha1) {
            newMap[i + 1] = make_pair(newList[i + 1].get(), j);
            oldMap[j] = make_pair(oldList[j].get(), i + 1);
        }
    }

    for (size_t i = newList.size() - 1; newList.size() > 0 && i > 0; --i) {
        if (!newMap[i].first || newMap[i - 1].first || newMap[i].second <= 0)
            continue;

        size_t j = newMap[i].second - 1;
        if (!oldMap[j].first && newList[i - 1]->m_sha1 == oldList[j]->m_sha1) {
            newMap[i - 1] = make_pair(newList[i - 1].get(), j);
            oldMap[j] = make_pair(oldList[j].get(), i - 1);
        }
    }

#ifdef DEBUG_DOM_PATCH_SUPPORT
    dumpMap(oldMap, "OLD");
    dumpMap(newMap, "NEW");
#endif

    return make_pair(oldMap, newMap);
}
