void ProfilingProcessHost::RequestProcessDump(base::ProcessId pid,
                                              base::FilePath dest,
                                              base::OnceClosure done) {
  if (!connector_) {
    DLOG(ERROR)
        << "Requesting process dump when profiling process hasn't started.";
    return;
  }

  base::PostTaskWithTraits(
      FROM_HERE, {base::TaskPriority::USER_VISIBLE, base::MayBlock()},
      base::BindOnce(&ProfilingProcessHost::GetOutputFileOnBlockingThread,
                     base::Unretained(this), pid, std::move(dest),
                     kNoTriggerName, std::move(done)));
}
