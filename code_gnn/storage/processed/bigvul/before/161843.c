void InitManagerMojoOnIO(mojom::DiscardableSharedMemoryManagerPtr* manager_mojo,
                         mojom::DiscardableSharedMemoryManagerPtrInfo info) {
  manager_mojo->Bind(std::move(info));
}
