std::string VariationsService::GetInvalidVariationsSeedSignature() const {
  DCHECK(thread_checker_.CalledOnValidThread());
  return seed_store_.GetInvalidSignature();
}
