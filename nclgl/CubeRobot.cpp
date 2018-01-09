#include "CubeRobot.h"

Mesh* CubeRobot::cube = NULL;

CubeRobot::CubeRobot(void) {
	// Optional , uncomment if you want a local origin marker !
	// SetMesh ( cube );

	SceneNode* body = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(body);
	body->SetBoundingRadius(15.0f);

	head = new SceneNode(cube, Vector4(0, 1, 0, 1)); // Green !
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	body->AddChild(head);
	head->SetBoundingRadius(5.0f);

	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftArm->SetModelScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body->AddChild(leftArm);
	leftArm->SetBoundingRadius(18.0f);

	rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightArm->SetModelScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	body->AddChild(rightArm);
	rightArm->SetBoundingRadius(18.0f);

	SceneNode* leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftLeg->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	body->AddChild(leftLeg);
	leftLeg->SetBoundingRadius(18.0f);

	SceneNode* rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightLeg->SetModelScale(Vector3(3, -17.5, 3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	body->AddChild(rightLeg);
	rightLeg->SetBoundingRadius(18.0f);
}

void CubeRobot::Update(float msec) {
	transform = transform *
		Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));

	head->SetTransform(head->GetTransform() *
		Matrix4::Rotation(-msec / 10.0f, Vector3(0, 1, 0)));


	leftArm->SetTransform(leftArm->GetTransform() *
		Matrix4::Rotation(-msec / 10.0f, Vector3(1, 0, 0)));

	rightArm->SetTransform(rightArm->GetTransform() *
		Matrix4::Rotation(msec / 10.0f, Vector3(1, 0, 0)));

	SceneNode::Update(msec);
}
