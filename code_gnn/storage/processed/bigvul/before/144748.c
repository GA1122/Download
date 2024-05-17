void NoopLocalSiteCharacteristicsDatabase::ReadSiteCharacteristicsFromDB(
    const url::Origin& origin,
    ReadSiteCharacteristicsFromDBCallback callback) {
  std::move(callback).Run(base::nullopt);
}
