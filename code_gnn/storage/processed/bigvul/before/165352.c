void DOMStorageContextWrapper::SetLocalStorageDatabaseForTesting(
    leveldb::mojom::LevelDBDatabaseAssociatedPtr database) {
  mojo_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&LocalStorageContextMojo::SetDatabaseForTesting,
                     base::Unretained(mojo_state_), std::move(database)));
}
