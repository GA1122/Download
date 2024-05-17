void DOMPatchSupport::dumpMap(const ResultMap& map, const String& name)
{
    fprintf(stderr, "\n\n");
    for (size_t i = 0; i < map.size(); ++i)
        fprintf(stderr, "%s[%lu]: %s (%p) - [%lu]\n", name.utf8().data(), i, map[i].first ? nodeName(map[i].first->m_node).utf8().data() : "", map[i].first, map[i].second);
}
