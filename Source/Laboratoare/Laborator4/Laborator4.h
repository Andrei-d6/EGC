#pragma once
#include <Component/SimpleScene.h>

class Laborator4 : public SimpleScene
{
public:
	Laborator4();
	~Laborator4();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat4 modelMatrix;
	float translateX, translateY, translateZ;
	float translateX1, translateY1, translateZ1;
	float scaleX, scaleY, scaleZ;
	float angularStepOX, angularStepOY, angularStepOZ;
	float angularStepOZ1;
	GLenum polygonMode;
	int sign1 = 1;
	int sign2 = 1;
};
