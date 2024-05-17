void IDNSpoofChecker::SetTopDomainGraph(base::StringPiece domain_graph) {
  DCHECK_NE(0u, domain_graph.length());
  g_graph = reinterpret_cast<const unsigned char*>(domain_graph.data());
  g_graph_length = domain_graph.length();
}
