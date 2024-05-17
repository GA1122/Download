void Resource::ClearRangeRequestHeader() {
  resource_request_.ClearHTTPHeaderField("range");
}
