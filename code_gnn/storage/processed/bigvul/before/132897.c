  int IterateAndCount(DrawQuad* quad) {
    num_resources_ = 0;
    quad->IterateResources(base::Bind(
        &DrawQuadIteratorTest::IncrementResourceId, base::Unretained(this)));
    return num_resources_;
  }
