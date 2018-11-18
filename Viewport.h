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

#endif /* _VIEWPORT_H_ */
