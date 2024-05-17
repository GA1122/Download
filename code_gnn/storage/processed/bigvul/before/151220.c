Response InspectorNetworkAgent::setDataSizeLimitsForTest(int max_total,
                                                         int max_resource) {
  resources_data_->SetResourcesDataSizeLimits(max_total, max_resource);
  return Response::OK();
}
