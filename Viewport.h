#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

class ArcBall;

class Viewport
{
public:
    Viewport();
    ~Viewport();

    void Draw();

private:
    ArcBall* camera_ctrl = nullptr;
};

#endif /* _VIEWPORT_H_ */
