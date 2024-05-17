std::string TabNodeIdToClientTag(const std::string& session_tag,
                                 int tab_node_id) {
  CHECK_GT(tab_node_id, TabNodePool::kInvalidTabNodeID);
  return base::StringPrintf("%s %d", session_tag.c_str(), tab_node_id);
}
