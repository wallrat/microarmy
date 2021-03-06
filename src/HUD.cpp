#include "Qor/Mesh.h"
#include "HUD.h"
using namespace std;
using namespace glm;

const std::vector<int> HUD :: STAR_LEVELS = { 7, 6, 2 };

HUD :: HUD(Window* window, Input* input, Cache<Resource,std::string>* cache):
    m_pWindow(window),
    m_pInput(input),
    m_pCache(cache)
{
    auto sw = m_pWindow->size().x;
    auto sh = m_pWindow->size().y;

    m_pCanvas = make_shared<Canvas>(sw, sh);
    add(m_pCanvas);
    
    set(0,0,0);
}

void HUD :: redraw()
{
    auto sw = m_pWindow->size().x;
    auto sh = m_pWindow->size().y;
    
    // clear transparent
    auto ctext = m_pCanvas->context();
    m_pCanvas->clear(Color(0.0f, 0.0f, 0.0f, 0.0f));
    
    auto layout = m_pCanvas->layout();
    layout->set_wrap(Pango::WRAP_WORD);
    
    m_FontDesc = Pango::FontDescription("Press Start 2P " + to_string(sw / 36));
    m_pCanvas->layout()->set_font_description(m_FontDesc);
    
    layout->set_text("  "+to_string(m_Stars)+"/"+to_string(m_MaxStars));
    ctext->set_source_rgba(1.0, 1.0, 1.0, 0.75);
    layout->show_in_cairo_context(ctext);

    m_pCanvas->dirty(true);
}

void HUD :: logic_self(Freq::Time)
{
    if(m_bDirty) {
        redraw();
        m_bDirty = false;
    }
}

void HUD :: set(int star_lev, int stars, int max_stars)
{
    if(star_lev != m_StarLev)
    {
        m_StarLev = star_lev;
        
        auto sw = m_pWindow->size().x;
        auto sh = m_pWindow->size().y;
        auto mat = make_shared<MeshMaterial>("items.png", m_pCache);
        if(m_pMesh)
            m_pMesh->detach();
        m_pMesh = make_shared<Mesh>(
            make_shared<MeshGeometry>(Prefab::quad(vec2(sw/24, sw/24))),
            vector<shared_ptr<IMeshModifier>>{
                make_shared<Wrap>(Prefab::tile_wrap(
                    // Y Y (height is tile size for both dims)
                    uvec2(mat->texture()->size().y, mat->texture()->size().y),
                    // X Y
                    uvec2(mat->texture()->size().x, mat->texture()->size().y),
                    STAR_LEVELS[m_StarLev]
                ))
            }, mat
        );
        add(m_pMesh);
    }
    m_Stars = stars;
    m_MaxStars = max_stars;
    m_bDirty = true;
}

