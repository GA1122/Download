bool TracingControllerImpl::GetCategories(GetCategoriesDoneCallback callback) {
  std::set<std::string> category_set;

  tracing::TraceEventAgent::GetInstance()->GetCategories(&category_set);
  for (auto& agent : agents_)
    agent->GetCategories(&category_set);

  std::move(callback).Run(category_set);
  return true;
}
