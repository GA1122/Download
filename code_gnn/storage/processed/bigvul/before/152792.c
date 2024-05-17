scoped_refptr<const NGLayoutResult> NGColumnLayoutAlgorithm::Layout() {
  LogicalSize border_box_size = container_builder_.InitialBorderBoxSize();
  LogicalSize content_box_size =
      ShrinkAvailableSize(border_box_size, border_scrollbar_padding_);
  LogicalSize column_size = CalculateColumnSize(content_box_size);

  WritingMode writing_mode = ConstraintSpace().GetWritingMode();

  LayoutUnit column_block_offset = IsResumingLayout(BreakToken())
                                       ? LayoutUnit()
                                       : border_scrollbar_padding_.block_start;

  LayoutUnit previously_used_block_size;
  if (const auto* token = BreakToken())
    previously_used_block_size = token->UsedBlockSize();

  bool needs_more_fragments_in_outer = false;
  if (ConstraintSpace().HasBlockFragmentation()) {
    column_size.block_size -= previously_used_block_size;

    LayoutUnit available_outer_space =
        ConstraintSpace().FragmentainerSpaceAtBfcStart() - column_block_offset;
    if (column_size.block_size > available_outer_space) {
      column_size.block_size = available_outer_space;
      needs_more_fragments_in_outer = true;
    }
  }
  LayoutUnit column_inline_progression =
      column_size.inline_size +
      ResolveUsedColumnGap(content_box_size.inline_size, Style());
  int used_column_count =
      ResolveUsedColumnCount(content_box_size.inline_size, Style());

  if (ConstraintSpace().HasBlockFragmentation())
    container_builder_.SetHasBlockFragmentation();

  do {
    scoped_refptr<const NGBlockBreakToken> break_token;
    if (const auto* token = BreakToken()) {
      const auto child_tokens = token->ChildBreakTokens();
      if (child_tokens.size()) {
        break_token =
            To<NGBlockBreakToken>(child_tokens[child_tokens.size() - 1]);
      }
    }

    LayoutUnit intrinsic_block_size;
    LayoutUnit column_inline_offset(border_scrollbar_padding_.inline_start);
    int actual_column_count = 0;
    int forced_break_count = 0;

    LayoutUnit minimal_space_shortage(LayoutUnit::Max());

    bool separate_leading_margins = true;

    do {
      NGConstraintSpace child_space = CreateConstraintSpaceForColumns(
          column_size, separate_leading_margins);

      NGFragmentGeometry fragment_geometry =
          CalculateInitialFragmentGeometry(child_space, Node());

      NGBlockLayoutAlgorithm child_algorithm(
          {Node(), fragment_geometry, child_space, break_token.get()});
      child_algorithm.SetBoxType(NGPhysicalFragment::kColumnBox);
      scoped_refptr<const NGLayoutResult> result = child_algorithm.Layout();
      const auto& column = result->PhysicalFragment();

      LogicalOffset logical_offset(column_inline_offset, column_block_offset);
      container_builder_.AddChild(column, logical_offset);

      LayoutUnit space_shortage = result->MinimalSpaceShortage();
      if (space_shortage > LayoutUnit()) {
        minimal_space_shortage =
            std::min(minimal_space_shortage, space_shortage);
      }
      actual_column_count++;
      if (result->HasForcedBreak()) {
        forced_break_count++;
        separate_leading_margins = true;
      } else {
        separate_leading_margins = false;
      }

      LayoutUnit block_size = NGFragment(writing_mode, column).BlockSize();
      intrinsic_block_size =
          std::max(intrinsic_block_size, column_block_offset + block_size);

      column_inline_offset += column_inline_progression;
      break_token = To<NGBlockBreakToken>(column.BreakToken());

      if (ConstraintSpace().HasBlockFragmentation() && break_token &&
          actual_column_count >= used_column_count &&
          needs_more_fragments_in_outer) {
        LayoutUnit fragment_block_size = child_space.FragmentainerBlockSize();
        LayoutUnit used_block_size = fragment_block_size;
        if (previously_used_block_size)
          used_block_size += previously_used_block_size;
        else
          fragment_block_size += border_scrollbar_padding_.block_start;
        container_builder_.SetUsedBlockSize(used_block_size);
        container_builder_.SetBlockSize(fragment_block_size);
        container_builder_.SetDidBreak();
        break;
      }
    } while (break_token);

    if (container_builder_.DidBreak())
      break;

    if (actual_column_count > used_column_count &&
        actual_column_count > forced_break_count + 1 &&
        minimal_space_shortage != LayoutUnit::Max()) {
      LayoutUnit new_column_block_size =
          StretchColumnBlockSize(minimal_space_shortage, column_size.block_size,
                                 content_box_size.block_size);

      DCHECK_GE(new_column_block_size, column_size.block_size);
      if (new_column_block_size > column_size.block_size) {
        column_size.block_size = new_column_block_size;
        container_builder_.RemoveChildren();
        continue;
      }
    }
    container_builder_.SetIntrinsicBlockSize(intrinsic_block_size);
    break;
  } while (true);


  if (!container_builder_.DidBreak()) {
    LayoutUnit block_size;
    if (border_box_size.block_size == kIndefiniteSize) {
      block_size =
          column_size.block_size + border_scrollbar_padding_.BlockSum();
    } else {
      block_size = border_box_size.block_size - previously_used_block_size;
    }
    container_builder_.SetBlockSize(block_size);
  }

  NGOutOfFlowLayoutPart(
      Node(), ConstraintSpace(),
      container_builder_.Borders() + container_builder_.Scrollbar(),
      &container_builder_)
      .Run();

  return container_builder_.ToBoxFragment();
}
