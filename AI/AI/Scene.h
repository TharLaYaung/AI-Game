#pragma once


enum class SceneType {
    OPENING,    
    TITLE,      
    CHARACTER_SELECT, 
    MAIN,       
    RESULT,     
    GAMEOVER,   
    RANKING,    
    LOADING,
    SETTINGS
};


class BaseScene {
public:
    virtual ~BaseScene() {}
    
    
    virtual void Initialize() = 0;
    
    
    virtual SceneType Update() = 0;
    
    
    virtual void Draw() = 0;
    
    
    virtual void Finalize() = 0;
};
