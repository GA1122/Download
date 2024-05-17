void DumpProcess(base::ProcessId pid, const base::FilePath& dumpfile_path) {
  profiling::ProfilingProcessHost* pph =
      profiling::ProfilingProcessHost::GetInstance();
  base::RunLoop run_loop;
  pph->RequestProcessDump(
      pid, dumpfile_path,
      base::BindOnce(&base::RunLoop::Quit, base::Unretained(&run_loop)));
  run_loop.Run();
}
