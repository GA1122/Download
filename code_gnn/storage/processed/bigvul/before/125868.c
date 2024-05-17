void ParamTraits<gfx::Vector2dF>::Log(const gfx::Vector2dF& v, std::string* l) {
  l->append(base::StringPrintf("(%f, %f)", v.x(), v.y()));
}
