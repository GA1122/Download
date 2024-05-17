void ProfilingProcessHost::OnProcessDumpComplete(base::FilePath file_path,
                                                 std::string trigger_name,
                                                 base::OnceClosure done,
                                                 bool success) {
  base::ScopedClosureRunner done_runner(std::move(done));
  if (!success) {
    DLOG(ERROR) << "Cannot dump process.";
    base::PostTaskWithTraits(
        FROM_HERE, {base::MayBlock(), base::TaskPriority::BACKGROUND},
        base::BindOnce(base::IgnoreResult(&base::DeleteFile), file_path,
                       false));
    return;
  }
}
