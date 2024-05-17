void LayoutNonRecursiveForTestingViewportIntersection(
    WebContents* web_contents) {
  static const char* script =
      "function relayoutNonRecursiveForTestingViewportIntersection() {\
        var width = window.innerWidth;\
        var height = window.innerHeight * 0.75;\
        for (var i = 0; i < window.frames.length; i++) {\
          child = document.getElementById(\"child-\" + i);\
          child.width = width;\
          child.height = height;\
        }\
      }\
      relayoutNonRecursiveForTestingViewportIntersection();";
  EXPECT_TRUE(ExecuteScript(web_contents, script));
}
