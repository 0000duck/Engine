#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

class ArcBall;
class Camera;

class Viewport
{
public:
    Viewport();
    ~Viewport();

    void Draw();
    void CenterCamera();

    const Camera*   GetCamera() const;
    unsigned        GetWidth () const;
    unsigned        GetHeight() const;

private:

    void GenerateFBOTexture(unsigned w, unsigned h);

private:

    ArcBall* camera_ctrl = nullptr;
    Camera*  camera      = nullptr;
    unsigned fbo         = 0;
    unsigned fb_depth    = 0;
    unsigned fb_tex      = 0;
    unsigned fb_width    = 0;
    unsigned fb_height   = 0;
};

inline const Camera* Viewport::GetCamera() const
{
    return camera;
}

inline unsigned Viewport::GetWidth () const
{
    return fb_width;
}

inline unsigned Viewport::GetHeight() const
{
    return fb_height;
}


#endif /* _VIEWPORT_H_ */
