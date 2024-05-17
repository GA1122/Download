  void TextTrack::addCue(TextTrackCue* cue) {
    DCHECK(cue);
  
  if (std::isnan(cue->startTime()) || std::isnan(cue->endTime()) ||
      cue->startTime() < 0 || cue->endTime() < 0)
//   if (std::isnan(cue->startTime()) || std::isnan(cue->endTime()))
      return;
  
 
 
 
   if (TextTrack* cue_track = cue->track())
     cue_track->removeCue(cue, ASSERT_NO_EXCEPTION);
 
   cue->SetTrack(this);
   EnsureTextTrackCueList()->Add(cue);
 
   if (GetCueTimeline() && mode_ != DisabledKeyword())
     GetCueTimeline()->AddCue(this, cue);
 }