#include "AIControllerComponent.h"

Ang3 CAIControllerComponent::CreateAnglesYPR(const Matrix33& m) {
	CRY_MATH_ASSERT(m.IsOrthonormal());
	float l = Vec3(m.m01, m.m11, 0.0f).GetLength();
	if (l > 0.0001)
		return Ang3(atan2f(-m.m01 / l, m.m11 / l), atan2f(m.m21, l), atan2f(-m.m20 / l, m.m22 / l));
	else
		return Ang3(0, atan2f(m.m21, l), 0);
}

Ang3 CAIControllerComponent::CreateAnglesYPR(const Vec3& vdir, f32 r) {
	CRY_MATH_ASSERT((fabs_tpl(1 - (vdir | vdir))) < 0.001);   //!< Check if unit vector.
	f32 l = Vec3(vdir.x, vdir.y, 0.0f).GetLength();           //!< Check if nonzero.
	if (l > 0.0001)
		return Ang3(atan2f(-vdir.x / l, vdir.y / l), atan2f(vdir.z, l), r);
	else
		return Ang3(0, atan2f(vdir.z, l), r);
}

Matrix33 CAIControllerComponent::CreateOrientationYPR(const Ang3 & ypr) {

	f32 sz, cz;
	sincos_tpl(ypr.x, &sz, &cz);
	f32 sx, cx;
	sincos_tpl(ypr.y, &sx, &cx);
	f32 sy, cy;
	sincos_tpl(ypr.z, &sy, &cy);
	Matrix33 c;
	c.m00 = cy * cz - sy * sz * sx;
	c.m01 = -sz * cx;
	c.m02 = sy * cz + cy * sz * sx;
	c.m10 = cy * sz + sy * sx * cz;
	c.m11 = cz * cx;
	c.m12 = sy * sz - cy * sx * cz;
	c.m20 = -sy * cx;
	c.m21 = sx;
	c.m22 = cy * cx;
	return c;
}

