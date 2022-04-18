# glfw_viewer
C++ wrapper for glfw api.

## Philosophy
1. This repo provides a class named 'viewer', it is a wrapped up of glfw window api (callbacks keyborad, mouse, resize, etc.)  and supports flexible __plugins__ .
2. You can add '__Plugin'__ into 'Viewer'.
3. In the mainloop, the draw action is split into three stage: pre-draw, drawAction, post-draw. You can define your three stage actions as you want.
4.  The viewer will excecute its __'pre-draw, drawAction, post-draw'__, and also excecute all enabled plugins' __'pre-draw, drawAction, post-draw'__.
## Sample usage
```
int main() {
	Viewer viewer;
	// Change default path
	fs::path workspace = fs::absolute(fs::current_path());
	// Initialize viewer
	if (viewer.launch_init(true, false, true, "viewer", 0, 0) == EXIT_FAILURE)
	{	viewer.launch_shut();
		return EXIT_FAILURE;
	}
    //how to add plugin:
	ViewerPlugin temp;
	viewer.plugins.push_back(&temp);
    
    //how to set up your draw action:
	viewer.setDrawCall([] () {
	  //set up your draw action here
		});
	// Rendering
	try
	{
		viewer.launch_rendering(true);
	}
	catch (...)
	{
		viewer.launch_shut();
	}
	viewer.launch_shut();
	return EXIT_SUCCESS;
}
```

## build
git clone this repository.
`cd GlfwViewer/`

`cmake ./`


