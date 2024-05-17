void ParamTraits<gfx::PointF>::Log(const gfx::PointF& v, std::string* l) {
  l->append(base::StringPrintf("(%f, %f)", v.x(), v.y()));
}
