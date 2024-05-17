void GDataCache::RequestInitializeOnUIThread() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  pool_->GetSequencedTaskRunner(sequence_token_)->PostTask(
      FROM_HERE,
      base::Bind(&GDataCache::Initialize, base::Unretained(this)));
}
