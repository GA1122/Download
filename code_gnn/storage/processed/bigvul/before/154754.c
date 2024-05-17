error::Error GLES2DecoderPassthroughImpl::DoMultiDrawEndCHROMIUM() {
  MultiDrawManager::ResultData result;
  if (!multi_draw_manager_->End(&result)) {
    return error::kInvalidArguments;
  }
  switch (result.draw_function) {
    case MultiDrawManager::DrawFunction::DrawArrays:
      api()->glMultiDrawArraysANGLEFn(result.mode, result.firsts.data(),
                                      result.counts.data(), result.drawcount);
      return error::kNoError;
    case MultiDrawManager::DrawFunction::DrawArraysInstanced:
      api()->glMultiDrawArraysInstancedANGLEFn(
          result.mode, result.firsts.data(), result.counts.data(),
          result.instance_counts.data(), result.drawcount);
      return error::kNoError;
    case MultiDrawManager::DrawFunction::DrawElements:
      api()->glMultiDrawElementsANGLEFn(result.mode, result.counts.data(),
                                        result.type, result.indices.data(),
                                        result.drawcount);
      return error::kNoError;
    case MultiDrawManager::DrawFunction::DrawElementsInstanced:
      api()->glMultiDrawElementsInstancedANGLEFn(
          result.mode, result.counts.data(), result.type, result.indices.data(),
          result.instance_counts.data(), result.drawcount);
      return error::kNoError;
    default:
      NOTREACHED();
      return error::kLostContext;
  }
}
