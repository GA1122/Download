void RenderFrameImpl::ShowContextMenu(const blink::WebContextMenuData& data) {
  ContextMenuParams params = ContextMenuParamsBuilder::Build(data);
  blink::WebRect position_in_window(params.x, params.y, 0, 0);
  GetRenderWidget()->ConvertViewportToWindow(&position_in_window);
  params.x = position_in_window.x;
  params.y = position_in_window.y;
  GetRenderWidget()->OnShowHostContextMenu(&params);
  if (GetRenderWidget()->has_host_context_menu_location()) {
    params.x = GetRenderWidget()->host_context_menu_location().x();
    params.y = GetRenderWidget()->host_context_menu_location().y();
  }

  if (params.src_url.spec().size() > url::kMaxURLChars)
    params.src_url = GURL();

  blink::WebRect selection_in_window(data.selection_rect);
  GetRenderWidget()->ConvertViewportToWindow(&selection_in_window);
  params.selection_rect = selection_in_window;

#if defined(OS_ANDROID)
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::Bind(&RenderFrameImpl::ShowDeferredContextMenu,
                            weak_factory_.GetWeakPtr(), params));
#else
  ShowDeferredContextMenu(params);
#endif
}
