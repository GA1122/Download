  void SetVideoType(MediaStreamType video_type) {
    DCHECK(IsVideoInputMediaType(video_type) || video_type == MEDIA_NO_SERVICE);
    video_type_ = video_type;
  }
