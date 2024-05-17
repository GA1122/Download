void UnregisterCallbackToClosure(
    const base::Closure& closure,
    content::mojom::PushUnregistrationStatus status) {
  DCHECK(!closure.is_null());
  closure.Run();
}
