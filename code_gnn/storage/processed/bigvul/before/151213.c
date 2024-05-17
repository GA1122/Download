Response InspectorNetworkAgent::enable(Maybe<int> total_buffer_size,
                                       Maybe<int> resource_buffer_size) {
  Enable(total_buffer_size.fromMaybe(g_maximum_total_buffer_size),
         resource_buffer_size.fromMaybe(g_maximum_resource_buffer_size));
  return Response::OK();
}
