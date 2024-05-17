void DataReductionProxyIOData::UpdateProxyRequestHeaders(
    const net::HttpRequestHeaders& headers) {
  ui_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&DataReductionProxyService::SetProxyRequestHeadersOnUI,
                     service_, std::move(headers)));
  UpdateCustomProxyConfig();
}
