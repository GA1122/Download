void BindJavaInterface(mojo::InterfaceRequest<Interface> request) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  content::GetGlobalJavaInterfaces()->GetInterface(std::move(request));
}
