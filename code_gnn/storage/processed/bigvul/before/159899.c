void RunOrPostGetMostVisitedURLsCallback(
    base::TaskRunner* task_runner,
    bool include_forced_urls,
    const TopSitesImpl::GetMostVisitedURLsCallback& callback,
    const MostVisitedURLList& all_urls,
    const MostVisitedURLList& nonforced_urls) {
  const MostVisitedURLList& urls =
      include_forced_urls ? all_urls : nonforced_urls;
  if (task_runner->RunsTasksInCurrentSequence())
    callback.Run(urls);
  else
    task_runner->PostTask(FROM_HERE, base::Bind(callback, urls));
}
