bool MultibufferDataSource::assume_fully_buffered() {
  return !url_data()->url().SchemeIsHTTPOrHTTPS();
}
