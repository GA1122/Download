bool UrlData::Valid() {
  DCHECK(thread_checker_.CalledOnValidThread());
  base::Time now = base::Time::Now();
  if (!range_supported_ && !FullyCached())
    return false;
  if (valid_until_ > now)
    return true;
  if (now - last_used_ <
      base::TimeDelta::FromSeconds(kUrlMappingTimeoutSeconds))
    return true;
  return false;
}
