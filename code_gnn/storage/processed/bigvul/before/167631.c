  void OnSetIsInert(bool is_inert) {
    content::BrowserThread::PostTask(
        content::BrowserThread::UI, FROM_HERE,
        base::BindOnce(&SetIsInertMessageFilter::OnSetIsInertOnUI, this,
                       is_inert));
  }
