Response EmulationHandler::SetGeolocationOverride(
    Maybe<double> latitude, Maybe<double> longitude, Maybe<double> accuracy) {
  if (!GetWebContents())
    return Response::InternalError();

  auto* geolocation_context = GetWebContents()->GetGeolocationContext();
  auto geoposition = device::mojom::Geoposition::New();
  if (latitude.isJust() && longitude.isJust() && accuracy.isJust()) {
    geoposition->latitude = latitude.fromJust();
    geoposition->longitude = longitude.fromJust();
    geoposition->accuracy = accuracy.fromJust();
    geoposition->timestamp = base::Time::Now();

    if (!device::ValidateGeoposition(*geoposition))
      return Response::Error("Invalid geolocation");

  } else {
    geoposition->error_code =
        device::mojom::Geoposition::ErrorCode::POSITION_UNAVAILABLE;
  }
  geolocation_context->SetOverride(std::move(geoposition));
  return Response::OK();
}
