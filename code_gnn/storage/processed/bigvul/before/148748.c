V4L2JpegEncodeAccelerator::V4L2JpegEncodeAccelerator(
    const scoped_refptr<base::SingleThreadTaskRunner>& io_task_runner)
    : child_task_runner_(base::ThreadTaskRunnerHandle::Get()),
      io_task_runner_(io_task_runner),
      client_(nullptr),
      encoder_thread_("V4L2JpegEncodeThread"),
      weak_factory_(this) {
  weak_ptr_ = weak_factory_.GetWeakPtr();
}
