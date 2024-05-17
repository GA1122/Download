BaseAudioContext::BaseAudioContext(Document* document,
                                   enum ContextType context_type)
    : PausableObject(document),
      destination_node_(nullptr),
      uuid_(CreateCanonicalUUIDString()),
      is_cleared_(false),
      is_resolving_resume_promises_(false),
      has_posted_cleanup_task_(false),
      deferred_task_handler_(DeferredTaskHandler::Create(
          document->GetTaskRunner(TaskType::kInternalMedia))),
      context_state_(kSuspended),
      periodic_wave_sine_(nullptr),
      periodic_wave_square_(nullptr),
      periodic_wave_sawtooth_(nullptr),
      periodic_wave_triangle_(nullptr),
      output_position_(),
      callback_metric_(),
      task_runner_(document->GetTaskRunner(TaskType::kInternalMedia)) {}
