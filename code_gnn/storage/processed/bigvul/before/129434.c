error::Error GLES2DecoderImpl::HandleScheduleOverlayPlaneCHROMIUM(
    uint32 immediate_data_size,
    const cmds::ScheduleOverlayPlaneCHROMIUM& c) {
  NOTIMPLEMENTED() << "Overlay supported isn't finished.";
  LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                     "glScheduleOverlayPlaneCHROMIUM",
                     "function not implemented");
  return error::kNoError;
}
