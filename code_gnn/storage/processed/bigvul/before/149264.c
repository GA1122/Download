void CrostiniUpgrader::Backup() {
  base::PostDelayedTask(
      FROM_HERE, {content::BrowserThread::UI},
      base::BindOnce(&CrostiniUpgrader::OnBackup,
                     weak_ptr_factory_.GetWeakPtr(), CrostiniResult::SUCCESS),
      base::TimeDelta::FromSeconds(5));
}
