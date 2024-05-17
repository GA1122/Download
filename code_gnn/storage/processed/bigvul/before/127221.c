UnprivilegedProcessDelegate::UnprivilegedProcessDelegate(
    scoped_refptr<base::SingleThreadTaskRunner> main_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    const FilePath& binary_path)
    : main_task_runner_(main_task_runner),
      io_task_runner_(io_task_runner),
      binary_path_(binary_path) {
}
