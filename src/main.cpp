//#include "ofMain.h"
//#include "ofApp.h"
//#include "ofAppGlutWindow.h"
//#include "matrix.h"
#include <iostream>
#include "viewer/Viewer.h"
#include <filesystem>

using namespace  std;
namespace   fs = std::filesystem;
//========================================================================
int main() {

	Viewer viewer;
	// Change default path
	//	viewer.path = "/path/to/workspace/";
	fs::path workspace = fs::absolute(fs::current_path());


	// General drawing settings
	/*viewer.core().is_animating = false;
	viewer.core().animation_max_fps = 30.;
	viewer.core().background_color = Eigen::Vector4f(0.6f, 0.6f, 0.6f, 1.0f);*/

	// Initialize viewer
	if (viewer.launch_init(true, false, true, "viewer", 0, 0) == EXIT_FAILURE)
	{
		viewer.launch_shut();
		return EXIT_FAILURE;
	}

	//init Graphics pipline
	/*std::filesystem::path name_inputObj = "assets\\objs\\KAUST_Beacon_NormalTexture.obj";
	auto inputObj = workspace / name_inputObj;
	OBjReader objFilereader;
	objFilereader.read( inputObj);
	auto triangles = objFilereader.TrianglesIdx;
	auto vertexs= objFilereader.Vertexes;
	RenderObject Sample(triangles, vertexs, objFilereader.VertexesNormal,objFilereader.VertexesTexture);
	GraphicsPipeline GP;*/

	viewer.setDrawCall([] (){
		//do the rendering here, this lambda function will be called for every frame in the main loop.
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










//int main() {
//	OBjReader objFilereader; 
//	string Path = string("C:\\Users\\8\\source\\repos\\assignment3\\assignment3\\KAUST_Beacon_Normal.obj");
//	string outPath = string("C:\\Users\\8\\source\\repos\\assignment3\\assignment3\\KAUST_Beacon_NormalTexture.obj");
//	//objFilereader.read(Path);
//	//objFilereader.Load_CalculateNormal_Store(Path, outPath);
//	objFilereader.Load_CalculateTextureCoordinate_Store(Path, outPath);
//	//RenderObject Sample(objFilereader.TrianglesIdx, objFilereader.Vertexes);
//	//GraphicsPipeline GP;
//	//GP.Render(Sample);
//
//	cout << "";
//}

