void GLES2DecoderImpl::DoMultiDrawEndCHROMIUM() {
  MultiDrawManager::ResultData result;
  if (!multi_draw_manager_->End(&result)) {
    MarkContextLost(error::kGuilty);
    group_->LoseContexts(error::kInnocent);
    return;
  }
  switch (result.draw_function) {
    case MultiDrawManager::DrawFunction::DrawArrays:
      DoMultiDrawArrays("glMultiDrawArraysWEBGL", false, result.mode,
                        result.firsts.data(), result.counts.data(), nullptr,
                        result.drawcount);
      break;
    case MultiDrawManager::DrawFunction::DrawArraysInstanced:
      DoMultiDrawArrays("glMultiDrawArraysInstancedWEBGL", true, result.mode,
                        result.firsts.data(), result.counts.data(),
                        result.instance_counts.data(), result.drawcount);
      break;
    case MultiDrawManager::DrawFunction::DrawElements:
      DoMultiDrawElements("glMultiDrawElementsWEBGL", false, result.mode,
                          result.counts.data(), result.type,
                          result.offsets.data(), nullptr, result.drawcount);
      break;
    case MultiDrawManager::DrawFunction::DrawElementsInstanced:
      DoMultiDrawElements("glMultiDrawElementsInstancedWEBGL", true,
                          result.mode, result.counts.data(), result.type,
                          result.offsets.data(), result.instance_counts.data(),
                          result.drawcount);
      break;
    default:
      NOTREACHED();
      MarkContextLost(error::kGuilty);
      group_->LoseContexts(error::kInnocent);
  }
}
