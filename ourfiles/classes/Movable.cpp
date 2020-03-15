#include "Movable.h"

Movable::Movable()
{
	Tout = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
	Tin = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
	Trot = Eigen::Transform<float, 3, Eigen::Affine>::Identity();
}


Eigen::Matrix4f Movable::MakeTrans()
{
	return Tout.matrix() * Trot.matrix() * Tin.matrix();
}

Eigen::Matrix4f Movable::GetRotationTrans()
{
	return Trot.matrix();
}

void Movable::MyTranslate(Eigen::Vector3f amt, bool preRotation)
{
	(preRotation == true) ? Tout.pretranslate(amt) : Tout.translate(amt);
}
//angle in radians
void Movable::MyRotate(Eigen::Vector3f rotAxis, float angle)
{
	Trot.rotate(Eigen::AngleAxisf(angle, rotAxis.normalized()));
}
void Movable::SetCenterOfRotation(Eigen::Vector3f amt)
{
	Tout.translate(amt);
	Tin.translate(-amt);
}

Eigen::Vector3f Movable::GetCenterOfRotation()
{
	return  -Tin.translation();

}
void Movable::MyScale(Eigen::Vector3f amt)
{
	Tout.scale(amt);
}


void Movable::TranslateInSystem(Eigen::Matrix4f mat, Eigen::Vector3f amt, bool preRotation)
{
	MyTranslate(mat.block<3, 3>(0, 0).transpose() * amt, preRotation);
}

void Movable::RotateInSystem(Eigen::Matrix4f mat, Eigen::Vector3f rotAxis, float angle, bool preRotation)
{
	MyRotate(mat.block<3, 3>(0, 0).transpose() * rotAxis, angle);
}
