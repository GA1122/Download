FFmpegVideoDecoder::FFmpegVideoDecoder(
    const scoped_refptr<base::SingleThreadTaskRunner>& task_runner)
    : task_runner_(task_runner), state_(kUninitialized) {}
