void ResourceMessageFilter::OnCloseCurrentConnections() {
  if (!CheckBenchmarkingEnabled())
    return;
  request_context_->GetURLRequestContext()->
      http_transaction_factory()->GetCache()->CloseCurrentConnections();
}
