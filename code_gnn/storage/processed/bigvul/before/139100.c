  FakeAudioManagerWithAssociations(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner,
      media::AudioLogFactory* factory)
      : FakeAudioManager(task_runner, task_runner, factory) {}
