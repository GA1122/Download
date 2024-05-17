 void DefaultAudioDestinationHandler::RestartRendering() {
   DCHECK(IsMainThread());
 
  StopRendering();
  StartRendering();
}
