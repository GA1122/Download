bool Histogram::AddSamplesFromPickle(PickleIterator* iter) {
  return samples_->AddFromPickle(iter);
}
