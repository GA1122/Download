void TestRenderFrame::Navigate(const network::ResourceResponseHead& head,
                               const CommonNavigationParams& common_params,
                               const CommitNavigationParams& commit_params) {
  if (!IsPerNavigationMojoInterfaceEnabled()) {
    CommitNavigation(head, common_params, commit_params,
                     network::mojom::URLLoaderClientEndpointsPtr(),
                     std::make_unique<blink::URLLoaderFactoryBundleInfo>(),
                     base::nullopt,
                     blink::mojom::ControllerServiceWorkerInfoPtr(),
                     blink::mojom::ServiceWorkerProviderInfoForWindowPtr(),
                     network::mojom::URLLoaderFactoryPtr(),
                     base::UnguessableToken::Create(), base::DoNothing());
  } else {
    BindNavigationClient(
        mojo::MakeRequestAssociatedWithDedicatedPipe(&mock_navigation_client_));
    CommitPerNavigationMojoInterfaceNavigation(
        head, common_params, commit_params,
        network::mojom::URLLoaderClientEndpointsPtr(),
        std::make_unique<blink::URLLoaderFactoryBundleInfo>(), base::nullopt,
        blink::mojom::ControllerServiceWorkerInfoPtr(),
        blink::mojom::ServiceWorkerProviderInfoForWindowPtr(),
        network::mojom::URLLoaderFactoryPtr(), base::UnguessableToken::Create(),
        base::BindOnce(&MockFrameHost::DidCommitProvisionalLoad,
                       base::Unretained(mock_frame_host_.get())));
  }
}
