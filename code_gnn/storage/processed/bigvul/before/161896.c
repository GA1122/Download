void PrepareFrameAndViewForPrint::DidStopLoading() {
  DCHECK(!on_ready_.is_null());
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::Bind(&PrepareFrameAndViewForPrint::CallOnReady,
                            weak_ptr_factory_.GetWeakPtr()));
}
