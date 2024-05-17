  void OnDeliverFrame(const scoped_refptr<media::VideoFrame>& video_frame,
                    base::TimeTicks estimated_capture_time) {
    DoOnDeliverFrame(video_frame, estimated_capture_time);
  }
