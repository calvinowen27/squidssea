#ifndef ANIMATION_INCLUDE
#define ANIMATION_INCLUDE

#include <vector>
#include <string>
#include <memory>

#include "../../SDL2/SDL.h"
#include "../Util/Vector2.hpp"

class Game;
class AnimationManager;
class ObjectManager;
class ContentManager;
enum class EntityType;

class Animation
{
private:
    Game &_game;
    AnimationManager &_animationManager;
    ObjectManager &_objectManager;
    ContentManager &_contentManager;

    EntityType _entityType;
    std::string _name;
    int _animationIdx;

    float _duration; // seconds
    int _frameCount;
    float _frameTime; // seconds, _duration / # of frames

    SDL_Texture *_pTexture;

    std::vector<SDL_Rect> _rects;

    bool _playing = false;
    bool _loops = true;

public:
    Animation(AnimationManager *animationManager);
    void init(EntityType entityType, std::string name, float duration, int frameCount, int animationIdx, bool loops);

    inline void play() { _playing = true; }
    inline void stop() { _playing = false; }
    inline bool isPlaying() { return _playing; }
    inline void setLoops(bool loops) { _loops = loops; }
    inline bool loops() { return _loops; }
    inline int getFrameCount() { return _frameCount; }
    inline float getDuration() { return _duration; }
    inline float getFrameTime() { return _frameTime; }
    inline std::vector<SDL_Rect> getRects() { return _rects; }
    inline SDL_Texture *getTexture() { return _pTexture; }
};

#endif