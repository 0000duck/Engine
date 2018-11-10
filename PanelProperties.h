#ifndef _PANEL_PROPERTIES_H_
#define _PANEL_PROPERTIES_H_

class PanelProperties
{
public:
    PanelProperties();
    ~PanelProperties();

    void Draw();

private:

    void DrawMesh(unsigned index);
    void DrawMaterial(unsigned index);
};

#endif /* _PANEL_PROPERTIES_H_ */
