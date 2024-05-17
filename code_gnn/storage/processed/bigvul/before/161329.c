Response NetworkHandler::Enable(Maybe<int> max_total_size,
                                Maybe<int> max_resource_size,
                                Maybe<int> max_post_data_size) {
  enabled_ = true;
  return Response::FallThrough();
}
