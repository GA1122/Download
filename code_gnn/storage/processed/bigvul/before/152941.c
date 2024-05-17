           media::ResolutionChangePolicy>::ToMojom(media::ResolutionChangePolicy
                                                       input) {
  switch (input) {
    case media::ResolutionChangePolicy::FIXED_RESOLUTION:
      return media::mojom::ResolutionChangePolicy::FIXED_RESOLUTION;
    case media::ResolutionChangePolicy::FIXED_ASPECT_RATIO:
      return media::mojom::ResolutionChangePolicy::FIXED_ASPECT_RATIO;
    case media::ResolutionChangePolicy::ANY_WITHIN_LIMIT:
      return media::mojom::ResolutionChangePolicy::ANY_WITHIN_LIMIT;
  }
  NOTREACHED();
  return media::mojom::ResolutionChangePolicy::FIXED_RESOLUTION;
}
