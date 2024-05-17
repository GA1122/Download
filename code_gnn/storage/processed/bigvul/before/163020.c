void ProfilingProcessHost::GetOutputFileOnBlockingThread(
    base::ProcessId pid,
    base::FilePath dest,
    std::string trigger_name,
    base::OnceClosure done) {
  base::ScopedClosureRunner done_runner(std::move(done));
  base::File file(dest,
                  base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_WRITE);
  content::BrowserThread::PostTask(
      content::BrowserThread::IO, FROM_HERE,
      base::BindOnce(&ProfilingProcessHost::HandleDumpProcessOnIOThread,
                     base::Unretained(this), pid, std::move(dest),
                     std::move(file), std::move(trigger_name),
                     done_runner.Release()));
}
