 void SetDroppableFrames(int num, unsigned int *list) {
 if (num > kMaxDroppableFrames)
      num = kMaxDroppableFrames;
 else if (num < 0)
      num = 0;
    droppable_nframes_ = num;
 for (unsigned int i = 0; i < droppable_nframes_; ++i)
      droppable_frames_[i] = list[i];
 }
