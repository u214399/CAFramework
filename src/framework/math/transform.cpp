#include "transform.h"
#include <math.h>

// Transforms can be combined in the same way as matrices and quaternions and the effects of two transforms can be combined into one transform
// To keep things consistent, combining transforms should maintain a right-to-left combination order
Transform combine(const Transform& t1, const Transform& t2)
{
	Transform out;
	out.scale = t1.scale * t2.scale;
	out.rotation = t2.rotation * t1.rotation; // right-to-left multiplication (right is the first rotation applyed)
	// The combined position needs to be affected by the rotation and scale
	out.position = t1.rotation * (t1.scale * t2.position); // M = R*S*T
	out.position = t1.position + out.position;
	return out;
}

Transform inverse(const Transform& t)
{
	Transform inv;
	inv.rotation = inverse(t.rotation);
	inv.scale.x = fabs(t.scale.x) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.x;
	inv.scale.y = fabs(t.scale.y) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.y;
	inv.scale.z = fabs(t.scale.z) < VEC3_EPSILON ? 0.0f : 1.0f / t.scale.z;
	vec3 invTrans = t.position * -1.0f;
	inv.position = inv.rotation * (inv.scale * invTrans);
	return inv;
}

Transform mix(const Transform& a, const Transform& b, float t)
{
	quat bRot = b.rotation;
	if (dot(a.rotation, bRot) < 0.0f) {
		bRot = -bRot;
	}
	return Transform(
		lerp(a.position, b.position, t),
		nlerp(a.rotation, bRot, t),
		lerp(a.scale, b.scale, t));
}

// Extract the rotation and the translition from a matrix is easy. But not for the scale
// M = SRT, ignore the translation: M = SR -> invert R to isolate S
Transform mat4_to_transform(const mat4& m)
{
	// TODO
	Transform result;

	// Extract the translation from the matrix
	result.position = vec3(m.position.x, m.position.y, m.position.z);

	// Extract the rotation from the matrix
	result.rotation = mat4_to_quat(m);
	
	// Compute the inverse rotation matrix then obtain the scale matrix S = MR^-1
	mat4 r_inverse;
	r_inverse.right = m.right;
	r_inverse.up = m.up;
	r_inverse.forward = m.forward;
	invert(r_inverse);
	mat4 scale;
	scale = m * r_inverse;

	result.scale.x = scale.right.x;
	result.scale.y = scale.up.y;
	result.scale.z = scale.forward.z;

	return result;
}

// Converts a transform into a mat4
mat4 transform_to_mat4(const Transform& t)
{
	// TODO
	// Create identity matrix
	mat4 result;

	// Set translation
	result.position = vec4(t.position, 1.f);

	// Multiply each unit vector by the quaternion to obtain the rotation in each axis
	vec3 right = t.rotation * vec3(1, 0, 0);
	vec3 up = t.rotation * vec3(0, 1, 0);
	vec3 forward = t.rotation * vec3(0, 0, 1); 
	
	// Add scale by multiplying the previously obtained vectors by the scale vector. (same as multiplying the matrix by a scale matrix with sx, sy, sz in diagonal)
	result.right = vec4(t.scale.x  * right,0.0f); // mutiply right by sx, up by sy, forward by sz
	result.up = vec4(t.scale.y * up, 0.0f);
	result.forward = vec4(t.scale.z * forward, 0.0f);

	return result;
}

vec3 transform_point(const Transform& a, const vec3& b)
{
	vec3 out;
	out = a.rotation * (a.scale * b);
	out = a.position + out;
	return out;
}

// First, apply the scale, then rotation
vec3 transform_vector(const Transform& t, const vec3& v)
{
	vec3 out;
	out = t.rotation * (t.scale * v);
	return out;
}