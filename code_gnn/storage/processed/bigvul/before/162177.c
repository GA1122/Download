MojoAudioOutputIPC::MojoAudioOutputIPC(FactoryAccessorCB factory_accessor)
    : factory_accessor_(std::move(factory_accessor)),
      binding_(this),
      weak_factory_(this) {
  DETACH_FROM_THREAD(thread_checker_);
}
