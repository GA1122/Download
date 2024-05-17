void ParamTraits<gfx::SizeF>::Log(const gfx::SizeF& p, std::string* l) {
  l->append(base::StringPrintf("(%f, %f)", p.width(), p.height()));
}
