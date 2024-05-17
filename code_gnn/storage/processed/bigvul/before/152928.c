    FromMojom(media::mojom::ResolutionChangePolicy input,
              media::ResolutionChangePolicy* output) {
  switch (input) {
    case media::mojom::ResolutionChangePolicy::FIXED_RESOLUTION:
      *output = media::ResolutionChangePolicy::FIXED_RESOLUTION;
      return true;
    case media::mojom::ResolutionChangePolicy::FIXED_ASPECT_RATIO:
      *output = media::ResolutionChangePolicy::FIXED_ASPECT_RATIO;
      return true;
    case media::mojom::ResolutionChangePolicy::ANY_WITHIN_LIMIT:
      *output = media::ResolutionChangePolicy::ANY_WITHIN_LIMIT;
      return true;
  }
  NOTREACHED();
  return false;
}
