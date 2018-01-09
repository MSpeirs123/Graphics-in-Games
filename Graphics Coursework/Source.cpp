#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "Renderer_Scene.h"


int main() {
	Window w("Coursework", 1280, 720, true); //This is all boring win32 window creation stuff!
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer_Scene renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}