void GDataCache::DestroyOnUIThread() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  ui_weak_ptr_factory_.InvalidateWeakPtrs();

  pool_->GetSequencedTaskRunner(sequence_token_)->PostTask(
      FROM_HERE,
      base::Bind(&GDataCache::Destroy,
                 base::Unretained(this)));
}
