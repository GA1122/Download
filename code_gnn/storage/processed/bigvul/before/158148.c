  static void MonitorRequestOnIoThread(
      const base::WeakPtr<RequestMonitoringNavigationBrowserTest>& weak_this,
      const scoped_refptr<base::SequencedTaskRunner>& postback_task_runner,
      const net::test_server::HttpRequest& request) {
    postback_task_runner->PostTask(
        FROM_HERE,
        base::BindOnce(
            &RequestMonitoringNavigationBrowserTest::MonitorRequestOnMainThread,
            weak_this, request));
  }
