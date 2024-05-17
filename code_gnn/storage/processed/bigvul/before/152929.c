    FromMojom(media::mojom::PowerLineFrequency input,
              media::PowerLineFrequency* output) {
  switch (input) {
    case media::mojom::PowerLineFrequency::DEFAULT:
      *output = media::PowerLineFrequency::FREQUENCY_DEFAULT;
      return true;
    case media::mojom::PowerLineFrequency::HZ_50:
      *output = media::PowerLineFrequency::FREQUENCY_50HZ;
      return true;
    case media::mojom::PowerLineFrequency::HZ_60:
      *output = media::PowerLineFrequency::FREQUENCY_60HZ;
      return true;
  }
  NOTREACHED();
  return false;
}
