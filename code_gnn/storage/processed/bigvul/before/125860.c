void ParamTraits<GURL>::Log(const GURL& p, std::string* l) {
  l->append(p.spec());
}
