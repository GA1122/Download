void RenderFrameImpl::ShowContextMenu(const blink::WebContextMenuData& data) {
  ContextMenuParams params = ContextMenuParamsBuilder::Build(data);
  blink::WebRect position_in_window(params.x, params.y, 0, 0);
  GetLocalRootRenderWidget()->ConvertViewportToWindow(&position_in_window);
  params.x = position_in_window.x;
  params.y = position_in_window.y;
  GetLocalRootRenderWidget()->OnShowHostContextMenu(&params);
  if (GetLocalRootRenderWidget()->has_host_context_menu_location()) {
    params.x = GetLocalRootRenderWidget()->host_context_menu_location().x();
    params.y = GetLocalRootRenderWidget()->host_context_menu_location().y();
  }

  if (params.src_url.spec().size() > url::kMaxURLChars)
    params.src_url = GURL();

  blink::WebRect selection_in_window(data.selection_rect);
  GetLocalRootRenderWidget()->ConvertViewportToWindow(&selection_in_window);
  params.selection_rect = selection_in_window;

#if defined(OS_ANDROID)
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&RenderFrameImpl::ShowDeferredContextMenu,
                                weak_factory_.GetWeakPtr(), params));
#else
  ShowDeferredContextMenu(params);
#endif
}
