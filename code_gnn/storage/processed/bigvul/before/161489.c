TargetHandler::TargetHandler()
    : DevToolsDomainHandler(Target::Metainfo::domainName),
      auto_attacher_(
          base::Bind(&TargetHandler::AutoAttach, base::Unretained(this)),
          base::Bind(&TargetHandler::AutoDetach, base::Unretained(this))),
      discover_(false),
      weak_factory_(this) {}
