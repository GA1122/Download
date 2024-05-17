  void FrameDelivered(const scoped_refptr<base::MessageLoopProxy>& loop,
                      base::Closure quit_closure,
                      base::TimeTicks timestamp,
                      bool frame_captured) {
    ++callback_invoke_count_;
    if (frame_captured)
      ++frames_captured_;
    if (!quit_closure.is_null())
      loop->PostTask(FROM_HERE, quit_closure);
  }
