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
        math::float3 panning;
	};

	Params     params;
	Params     dragging;

public:

	ArcBall();
	~ArcBall();

	static ArcBall* GetService();

	void Update         (unsigned elapsed, Camera* camera = nullptr);

	void SetRadius      (float radius);
	void SetPolar       (float polar);
	void SetAzimuthal   (float azimuthal);
    void SetPanning     (const math::float3& panning);

	void UpdateCamera   (Camera* camera);
};

#endif /* _ARCBALL_H_ */
