bool ParamTraits<content::Referrer>::Read(
    const Message* m, PickleIterator* iter, param_type* r) {
  return ReadParam(m, iter, &r->url) && ReadParam(m, iter, &r->policy);
}
