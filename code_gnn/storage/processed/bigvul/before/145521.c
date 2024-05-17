void RecordAbortRapporOnUI(const GURL& url,
                           base::TimeDelta request_loading_time) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (request_loading_time.InMilliseconds() < 100)
    GetContentClient()->browser()->RecordURLMetric("Net.ErrAborted.Fast", url);
  else
    GetContentClient()->browser()->RecordURLMetric("Net.ErrAborted.Slow", url);
}
