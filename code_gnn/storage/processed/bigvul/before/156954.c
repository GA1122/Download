  void EnqueueFrame(const scoped_refptr<media::VideoFrame>& frame) {
    DCHECK(io_thread_checker_.CalledOnValidThread());

    bool tracing_enabled = false;
    TRACE_EVENT_CATEGORY_GROUP_ENABLED("media", &tracing_enabled);
    if (tracing_enabled) {
      base::TimeTicks render_time;
      if (frame->metadata()->GetTimeTicks(
              media::VideoFrameMetadata::REFERENCE_TIME, &render_time)) {
        TRACE_EVENT1("media", "EnqueueFrame", "Ideal Render Instant",
                     render_time.ToInternalValue());
      } else {
        TRACE_EVENT0("media", "EnqueueFrame");
      }
    }

    const bool is_opaque = media::IsOpaque(frame->format());
    media::VideoRotation video_rotation = media::VIDEO_ROTATION_0;
    ignore_result(frame->metadata()->GetRotation(
        media::VideoFrameMetadata::ROTATION, &video_rotation));

    if (!received_first_frame_) {
      received_first_frame_ = true;
      last_frame_opaque_ = is_opaque;
      last_frame_rotation_ = video_rotation;
      main_task_runner_->PostTask(
          FROM_HERE, base::BindOnce(&WebMediaPlayerMS::OnFirstFrameReceived,
                                    player_, video_rotation, is_opaque));
    } else {
      if (last_frame_opaque_ != is_opaque) {
        last_frame_opaque_ = is_opaque;
        main_task_runner_->PostTask(
            FROM_HERE, base::BindOnce(&WebMediaPlayerMS::OnOpacityChanged,
                                      player_, is_opaque));
      }
      if (last_frame_rotation_ != video_rotation) {
        last_frame_rotation_ = video_rotation;
        main_task_runner_->PostTask(
            FROM_HERE, base::BindOnce(&WebMediaPlayerMS::OnRotationChanged,
                                      player_, video_rotation, is_opaque));
      }
    }

    enqueue_frame_cb_.Run(frame);
  }
