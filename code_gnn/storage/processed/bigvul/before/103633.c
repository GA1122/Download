void ChromeContentBrowserClient::WorkerProcessHostCreated(
    WorkerProcessHost* host) {
  host->AddFilter(new ChromeWorkerMessageFilter(host));
}
