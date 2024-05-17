  gfx::PointF GetPointInChild() {
    gfx::PointF point_f;
    std::string str;
    EXPECT_TRUE(ExecuteScriptAndExtractString(
        child_frame_tree_node_->current_frame_host(), "get_point_inside_text()",
        &str));
    ConvertJSONToPoint(str, &point_f);
    point_f = child_rwhv()->TransformPointToRootCoordSpaceF(point_f);
    return point_f;
  }
