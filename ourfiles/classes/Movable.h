#pragma once
#include <Eigen/core>
#include <Eigen/Geometry>


class Movable
{
public:
	Movable();
	Eigen::Matrix4f MakeTrans();
	void MyTranslate(Eigen::Vector3f amt, bool preRotation);
	void MyRotate(Eigen::Vector3f rotAxis, float angle);
	void MyScale(Eigen::Vector3f amt);
	void SetCenterOfRotation(Eigen::Vector3f amt);
	Eigen::Vector3f GetCenterOfRotation();
	Eigen::Matrix4f GetRotationTrans();
	void TranslateInSystem(Eigen::Matrix4f mat, Eigen::Vector3f amt, bool preRotation);
	void RotateInSystem(Eigen::Matrix4f mat, Eigen::Vector3f rotAxis, float angle, bool preRotation);


	Eigen::Transform<float, 3, Eigen::Affine> Tout;
	Eigen::Transform<float, 3, Eigen::Affine> Tin;
	Eigen::Transform<float, 3, Eigen::Affine> Trot;

};

