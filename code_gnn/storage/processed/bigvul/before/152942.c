    media::PowerLineFrequency>::ToMojom(media::PowerLineFrequency input) {
  switch (input) {
    case media::PowerLineFrequency::FREQUENCY_DEFAULT:
      return media::mojom::PowerLineFrequency::DEFAULT;
    case media::PowerLineFrequency::FREQUENCY_50HZ:
      return media::mojom::PowerLineFrequency::HZ_50;
    case media::PowerLineFrequency::FREQUENCY_60HZ:
      return media::mojom::PowerLineFrequency::HZ_60;
  }
  NOTREACHED();
  return media::mojom::PowerLineFrequency::DEFAULT;
}
