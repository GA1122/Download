PaintPropertyTreeBuilder::ContextForFragment(
    const base::Optional<LayoutRect>& fragment_clip,
    LayoutUnit logical_top_in_flow_thread) const {
  const auto& parent_fragments = context_.fragments;
  if (parent_fragments.IsEmpty())
    return PaintPropertyTreeBuilderFragmentContext();

  LayoutUnit logical_top_in_containing_flow_thread;

  if (object_.IsLayoutFlowThread()) {
    const auto& flow_thread = ToLayoutFlowThread(object_);
    logical_top_in_containing_flow_thread =
        FragmentLogicalTopInParentFlowThread(flow_thread,
                                             logical_top_in_flow_thread);
    for (const auto& parent_context : parent_fragments) {
      if (logical_top_in_containing_flow_thread ==
          parent_context.logical_top_in_flow_thread) {
        auto context = parent_context;
        context.fragment_clip = fragment_clip;
        context.logical_top_in_flow_thread = logical_top_in_flow_thread;
        return context;
      }
    }
  } else {
    bool parent_is_under_same_flow_thread;
    auto* pagination_layer =
        context_.painting_layer->EnclosingPaginationLayer();
    if (object_.IsColumnSpanAll()) {
      parent_is_under_same_flow_thread = false;
    } else if (object_.IsOutOfFlowPositioned()) {
      parent_is_under_same_flow_thread =
          (object_.Parent()->PaintingLayer()->EnclosingPaginationLayer() ==
           pagination_layer);
    } else {
      parent_is_under_same_flow_thread = true;
    }

    if (parent_is_under_same_flow_thread) {
      DCHECK(object_.Parent()->PaintingLayer()->EnclosingPaginationLayer() ==
             pagination_layer);
      for (const auto& parent_context : parent_fragments) {
        if (logical_top_in_flow_thread ==
            parent_context.logical_top_in_flow_thread) {
          auto context = parent_context;
          context.fragment_clip = base::nullopt;
          return context;
        }
      }
    }

    logical_top_in_containing_flow_thread = logical_top_in_flow_thread;
  }

  auto context = parent_fragments[0];
  context.logical_top_in_flow_thread = logical_top_in_flow_thread;
  context.fragment_clip = fragment_clip;

  for (const auto* container = object_.Container(); container;
       container = container->Container()) {
    if (!container->FirstFragment().HasLocalBorderBoxProperties())
      continue;

    for (const auto* fragment = &container->FirstFragment(); fragment;
         fragment = fragment->NextFragment()) {
      if (fragment->LogicalTopInFlowThread() ==
          logical_top_in_containing_flow_thread) {
        DCHECK(fragment->PostOverflowClip());
        context.current.clip = fragment->PostOverflowClip();
        return context;
      }
    }

    if (container->IsLayoutFlowThread()) {
      logical_top_in_containing_flow_thread =
          FragmentLogicalTopInParentFlowThread(
              ToLayoutFlowThread(*container),
              logical_top_in_containing_flow_thread);
    }
  }

  NOTREACHED();
  return context;
}
