#ifndef PARALLAX_LAYER_H
#define PARALLAX_LAYER_H

#include "CGameObject.h"
#include "TileableSprite.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
class ParallaxLayer : public CGameObject
{
public:
	/*-------------------------------------------------------------------------
	 Enumerators
	-------------------------------------------------------------------------*/
	enum DIRECTION
	{ 
		DIR_FORWARD	 = 1,
		DIR_BACKWARD = 2,
		DIR_LEFT     = 4,
		DIR_RIGHT    = 8,
	};

    enum AXIS
    {
        AXIS_HORIZONTAL = 1,
        AXIS_VERTICAL = 2
    };

    /*-------------------------------------------------------------------------
	 Constructors & Destructors for This Class.
	-------------------------------------------------------------------------*/
			 ParallaxLayer(const char *szImageFile);
	virtual ~ParallaxLayer();

	/*-------------------------------------------------------------------------
	 Public Functions for This Class.
	-------------------------------------------------------------------------*/
    void            Initialize(ULONG ulAxis, int iLayerSpeed, ULONG ulViewWidth, ULONG ulViewHeight);
    virtual void    Update(float dt);
    void            Move(ULONG ulDirection);
    virtual void    Draw() const { m_pTile->Draw(); }
    virtual bool    AffectedByGravity() const { return false; }
    virtual int     GetWidth() const { return m_pTile->GetWidth(); }
    virtual int     GetHeight() const { return m_pTile->GetHeight(); }

private:
	/*-------------------------------------------------------------------------
	 Private Variables for This Class.
	-------------------------------------------------------------------------*/
    int             m_iLayerSpeed;
    ULONG           m_nViewWidth;
    ULONG           m_nViewHeight;
    TileableSprite* m_pTile;
};

#endif // PARALLAX_LAYER_H