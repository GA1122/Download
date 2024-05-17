void CrostiniUpgrader::Upgrade(const ContainerId& container_id) {
  container_id_ = container_id;
  CrostiniManager::GetForProfile(profile_)->UpgradeContainer(
      container_id_, ContainerVersion::STRETCH, ContainerVersion::BUSTER,
      base::BindOnce(&CrostiniUpgrader::OnUpgrade,
                     weak_ptr_factory_.GetWeakPtr()));
}
