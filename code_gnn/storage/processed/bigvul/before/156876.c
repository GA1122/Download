DocumentInit DocumentInit::CreateWithImportsController(
    HTMLImportsController* controller) {
  DCHECK(controller);
  Document* master = controller->Master();
  return DocumentInit(controller)
      .WithContextDocument(master->ContextDocument())
      .WithRegistrationContext(master->RegistrationContext());
}
