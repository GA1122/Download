void OnCompositePdf(
    printing::mojom::PdfCompositorPtr compositor,
    printing::mojom::PdfCompositor::CompositePdfCallback callback,
    scoped_refptr<base::SequencedTaskRunner> task_runner,
    mojom::PdfCompositor::Status status,
    mojo::ScopedSharedBufferHandle pdf_handle) {
  task_runner->PostTask(FROM_HERE, base::BindOnce(std::move(callback), status,
                                                  base::Passed(&pdf_handle)));
}
