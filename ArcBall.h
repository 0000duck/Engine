#ifndef _ARCBALL_H_
#define _ARCBALL_H_

#include "MathGeoLib.h"

class Camera;

class ArcBall
{
	struct Params
	{
		float 	     radius = 0.0f;
		float 	     polar = 0.0f;
		float        azimuthal = 0.0f;
        math::float3 panning = math::float3::zero;
	};

	Params     params;
	Params     dragging;

public:

	ArcBall();
	~ArcBall();

	void                Update         (Camera* camera = nullptr);

	void                SetRadius      (float radius);
	void                SetPolar       (float polar);
	void                SetAzimuthal   (float azimuthal);
    void                SetPanning     (const math::float3& panning);

	float               GetRadius      () const;
	float               GetPolar       () const;
	float               GetAzimuthal   () const;
    const math::float3& GetPanning     () const;

	void                UpdateCamera   (Camera* camera);
};

inline float ArcBall::GetRadius() const
{
    return params.radius;
}

inline float ArcBall::GetPolar() const
{
    return params.polar;
}

inline float ArcBall::GetAzimuthal() const
{
    return params.azimuthal;
}

inline const math::float3& ArcBall::GetPanning() const
{
    return params.panning;
}

#endif /* _ARCBALL_H_ */
