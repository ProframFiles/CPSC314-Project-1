#pragma once

#define AKJ_PI 3.1415926535897932384626433832795028842
#define AKJ_PIf 3.1415926535897932384626433832795028842f
#define AKJ_INV_PI  0.318309886183791
#define AKJ_INV_PIf  0.318309886183791f
#define INV_360 0.00277777777777777777777777777777
#define INV_360f 0.00277777777777777777777777777777f
#include <math.h>
#include <cassert>


inline double ToRadians(double degrees)
{
	double wholepart = 0.0;
	const double frac_part = fmod(degrees*INV_360+0.5, 1.0) - 0.5 ;
	return frac_part * (2.0 * AKJ_PI);
}
inline float ToRadians(float degrees)
{
	float wholepart = 0.0f;
	const float frac_part = fmodf(degrees*INV_360f+0.5f, 1.0f) - 0.5f ;
	return frac_part * (2.0f * AKJ_PIf);
}
inline float ToDegrees(float radians)
{
	const float frac_part = (fmodf(radians + AKJ_PIf, 2.0f*AKJ_PIf) - AKJ_PIf) * AKJ_INV_PIf  ;
	return frac_part * 180.0f;
}
inline double ToDegrees(double radians)
{
	const double frac_part = (fmod(radians + AKJ_PI, 2.0*AKJ_PI) - AKJ_PI) * AKJ_INV_PI  ;
	return frac_part * 180.0;
}
struct cCoord2
{
	cCoord2():x(0.0f),y(0.0f){};
	cCoord2(float in_x, float in_y):x(in_x),y(in_y){};
	float x;
	float y;
};
inline cCoord2 operator +(const cCoord2& lhs, const cCoord2& rhs)
{
	return cCoord2(lhs.x+rhs.x, lhs.y+rhs.y);
}
inline cCoord2 operator -(const cCoord2& lhs, const cCoord2& rhs)
{
	return cCoord2(lhs.x-rhs.x, lhs.y-rhs.y);
}

//convenience class for working with 3 dimensional coordinates.
struct cCoord3
{
	cCoord3():x(0.0f),y(0.0f),z(0.0f){};
	explicit cCoord3(float n):x(n), y(n), z(n){};
	cCoord3(float in_x, float in_y, float in_z):x(in_x),y(in_y),z(in_z){};
	cCoord3(const cCoord3& rhs):x(rhs.x),y(rhs.y),z(rhs.z){};
	const cCoord3& operator=(const cCoord3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	inline float lengthSquared() const
	{
		return x*x + y*y + z*z;
	}
	inline float length() const
	{
		const float l =sqrt(lengthSquared());
		assert(l==l);
		return l;
	}
	cCoord3 normalized() const
	{
		const float l = length();
		return cCoord3(x/l, y/l, z/l);
	}

	cCoord3 cross(const cCoord3& rhs) const
	{
		return cCoord3(y*rhs.z - rhs.y*z, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
	}
	float dot(const cCoord3& rhs) const
	{
		return rhs.x*x+rhs.y*y+rhs.z*z;
	}
	inline static cCoord3 Zeros()
	{
		return cCoord3(0.0f, 0.0f, 0.0f);
	}
	inline static cCoord3 Ones()
	{
		return cCoord3(1.0f, 1.0f, 1.0f);
	}
	inline static cCoord3 XAxis()
	{
		return cCoord3(1.0f, 0.0f, 0.0f);
	}
	inline static cCoord3 YAxis()
	{
		return cCoord3(0.0f, 1.0f, 0.0f);
	}
	inline static cCoord3 ZAxis()
	{
		return cCoord3(0.0f, 0.0f, 1.0f);
	}
	cCoord3 least_axis() const
	{
		float ax = fabs(x);
		float ay = fabs(y);
		float az = fabs(z);

		if(ax < ay)
		{
			if(az < ax)
			{
				return cCoord3::ZAxis();
			}
			else
			{
				return cCoord3::XAxis();
			}
		}
		else if(ay < az)
		{
			return cCoord3::YAxis();
		}
		return cCoord3::ZAxis();
	}
	float x;
	float y;
	float z;
};
inline cCoord3 operator+(const cCoord3& lhs, const cCoord3& rhs)
{
	return cCoord3(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
}
inline cCoord3 operator-(const cCoord3& lhs, const cCoord3& rhs)
{
	return cCoord3(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
}

inline cCoord3 operator*(float factor, const cCoord3& rhs)
{
	return cCoord3(factor*rhs.x, factor*rhs.y, factor*rhs.z);
}
inline cCoord3 operator*(const cCoord3& lhs, const cCoord3& rhs)
{
	return cCoord3(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z);
}
inline cCoord3 operator/(const cCoord3& lhs, const cCoord3& rhs)
{
	return cCoord3(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z);
}
class cAngleAxis
{
public:
	cAngleAxis():angle(0.0f),x(0.0f),y(0.0f),z(1.0f){
		normalizeAxis();
	};
	cAngleAxis(float in_radians, const cCoord3& axis):angle(in_radians), x(axis.x), y(axis.y), z(axis.z){}
	cAngleAxis(float in_radians, float in_x, float in_y, float in_z):angle(in_radians),x(in_x),y(in_y),z(in_z){
		normalizeAxis();
	};

	void normalizeAxis()
	{
		const float inv_length = 1.0f/sqrt(x*x + y*y + z*z);
		x *= inv_length;
		y *= inv_length;
		z *= inv_length;
	}
	
	float angle;
	float x;
	float y;
	float z;
};

// never managed to finish this one
class cMatrix4f
{
public:
	cMatrix4f()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if(i==j)
				{
					at(i, j) = 1.0f;
				}
				else
				{
					at(i, j) = 0.0f;
				}
			}
		} 
	}
	inline float& at(int row, int col)
	{
		return mData[row+col*4];
	}
	inline float& operator()(int row, int col)
	{
		return mData[row+col*4];
	}
	float* Data()
	{
		return mData;
	}
private:
	float mData[16];
};


// quaternion class
// most of the math here was either guessed at/derived by myself,
// and most of the ideas come from trawling wikipedia and misc. forum postings
class cUnitQuat
{
public:
	cUnitQuat():s(0.0f, 0.0f, 0.0f),w(1.0f){}
	cUnitQuat(float wadda,const cCoord3& vector):s(vector),w(wadda){
		assert(wadda==wadda);
		const float ls = s.lengthSquared();
		if(ls == 0.0f || w == 0.0f)
		{
			w = 1.0f;
			s = cCoord3(0.0f, 0.0f, 0.0f);
		}
		else if(ls + w*w - 1.0f > 0.0005f)
		{
			s = sqrt( (1.0f-w*w)/s.lengthSquared()) * s;
		}
	}
	explicit cUnitQuat(const cAngleAxis& aa)
	{
		const float a_by_2 = aa.angle*0.5f;
		s = sin(a_by_2)*cCoord3(aa.x, aa.y, aa.z).normalized();
		w = cos(a_by_2);
	}
	cUnitQuat(const cCoord3& from, const cCoord3& to)
	{
		const cCoord3 from_normed = from.normalized();
		const cCoord3 to_normed = to.normalized();
		const cCoord3 m = (from_normed + to_normed).normalized();
		s = m.cross(to_normed);
		w = m.dot(to_normed);
	}
	cCoord3 rotate(const cCoord3& v) const
	{
		return v + 2.0f * s.cross( s.cross(v) + w*v );
	}
	cCoord3 rev_rotate(const cCoord3& v) const
	{
		return v + 2.0f * s.cross( s.cross(v) - w*v );
	}
	cAngleAxis to_angle_axis() const
	{
		if(w >= 1.0)
		{
			return cAngleAxis(0.0f, 0.0f, 0.0f, 1.0f);
		}
		const float angle = 2.0f*atan2(s.length(), w);
		return cAngleAxis(angle, s);
	}
	cUnitQuat scale(float factor) const
	{
		if(w >= 1.0f)
		{
			return cUnitQuat();
		}
		const float half_theta = acosf(w)*factor;
		const float sign_theta = half_theta > 0 ? 1.0f: -1.0f;
		float new_w = cos(half_theta);
		return cUnitQuat(sign_theta*new_w, sqrt( (1.0f-new_w*new_w)/s.lengthSquared() ) * s );
	}
	inline static cUnitQuat YZ()
	{
		return cUnitQuat(0.707106769f, cCoord3(0.707106769f, 0.0f, 0.0f));
	}
	inline static cUnitQuat XZ()
	{
		return cUnitQuat(0.707106769f, cCoord3(0.0f, 0.707106769f, 0.0f));
	}
	inline static cUnitQuat YX()
	{
		return cUnitQuat(0.707106769f, cCoord3(0.0f, 0.0f, 0.707106769f));
	}
	inline static cUnitQuat ZY()
	{
		return cUnitQuat(0.707106769f, cCoord3(-0.707106769f, 0.0f, 0.0f));
	}
	inline static cUnitQuat ZX()
	{
		return cUnitQuat(0.707106769f, cCoord3(0.0f, -0.707106769f, 0.0f));
	}
	inline static cUnitQuat XY()
	{
		return cUnitQuat(0.707106769f, cCoord3(0.0f, 0.0f, -0.707106769f));
	}
	cCoord3 s;
	float w;
};
inline cUnitQuat operator*(const cUnitQuat& lhs, const cUnitQuat& rhs)
{
	return cUnitQuat(lhs.w*rhs.w - lhs.s.dot(rhs.s), lhs.w*rhs.s + rhs.w*lhs.s + lhs.s.cross(rhs.s));
}

inline cUnitQuat Slerp(const cUnitQuat& lhs, const cUnitQuat& rhs, float t)
{
	return lhs.scale(1.0f-t)*rhs.scale(t);
}
// approximation for the circumference of an ellipse. Lifted from wikipedia
inline float EllipseCircumference(float semi_major, float semi_minor)
{
	const float diff_ab = (semi_major - semi_minor);
	const float sum_ab = (semi_major + semi_minor);
	const float q  = (diff_ab*diff_ab)/(sum_ab*sum_ab);
	return AKJ_PIf*sum_ab*(1.0f+(3.0f*q)/(10.0f + sqrtf(4.0f - 3.0f*q)) );
}