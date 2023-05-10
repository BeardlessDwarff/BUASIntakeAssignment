#pragma once
#include "surface.h"
#include "template.h"
#include <list>
#include <map>

using namespace Tmpl8;

struct LevelBlock
{
    enum class BlockType 
    {
        regular,
        bouncy,
        sticky,
        finish
    };

    BlockType type = BlockType::regular;
    bool visible = true;
    bool solid = true;


    float x;
    float y;
    float width;
    float height;

    vec4 lines[4];
    vec4 topLine;
    vec4 rightLine;
    vec4 bottomLine;
    vec4 leftLine;

    //map of BlockType to color. static since if we ever do change the colors for type this should be done for every block
    static const std::map<BlockType, uint32_t> blockTypeToColor;
    uint32_t color;

    //Constructor point + width + height
    LevelBlock(float x, float y, float width, float height, BlockType type = BlockType::regular)
    {
        // set values
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->type = type;


        // set the color based on block type
        color = blockTypeToColor.at(type);

        // create lines based on the width and height. these are usefull for collision
        topLine = vec4(x, y, x + width, y);
        rightLine = vec4(x + width, y,  x + width, y + height);
        bottomLine = vec4(x, y + height,  x + width, y + height);
        leftLine = vec4(x, y, x, y + height);

        lines[0] = topLine;
        lines[1] = rightLine;
        lines[2] = bottomLine;
        lines[3] = leftLine;
    }

    LevelBlock(vec4 topLine, vec4 rightLine, vec4 bottomLine, vec4 leftLine, BlockType type = BlockType::regular)
    {
        this->topLine = topLine;
        this->rightLine = rightLine;
        this->bottomLine = bottomLine;
        this->leftLine = leftLine;
        this->type = type;
        
        color = blockTypeToColor.at(type);

        x = topLine.x;
        y = topLine.y;
        width = bottomLine.z;
        height = bottomLine.w;

        lines[0] = topLine;
        lines[1] = rightLine;
        lines[2] = bottomLine;
        lines[3] = leftLine;
    }

    void Draw(Surface* screen)
    {
        if (!visible) return;
        screen->Box((int)x, (int)y, (int)(x + width), (int)(y + height), color);
    }

    void DrawFilled(Surface* screen)
    {
        if (!visible) return;
        screen->Bar((int)x, (int)y, (int)(x + width), (int)(y + height), color);
    }

    void DrawSprite(Surface* screen, Surface* sprite)
    {

    }

    
    // TODO
    //Moved this to Level.cpp. I had a linker error. not 100% yet why but  figure out how this worked exactly.
    //const std::map<levelBlock::BlockType, uint32_t> levelBlock::blockTypeToColor =
    //{
    //    {levelBlock::BlockType::regular, 0xFF0000}, //red
    //    {levelBlock::BlockType::bouncy, 0x00FF00}, //greeb
    //    {levelBlock::BlockType::sticky, 0x0000FF} //blue
    //};

};

struct Finish {

    LevelBlock baseBlock;
    LevelBlock finishBlock;

    //TODO currently you have to send finish block aswel
    //i want to make a constructor that passes along a enum with left, right, middle
    //based on this enum in the contructor we will fill in the finish block.

    Finish(const LevelBlock& base, const LevelBlock& finish)
        : baseBlock(base), finishBlock(finish) 
    {

        //SHOW finish border in debug
#ifdef NDEBUG
        finishBlock.visible = true; //TODO make this false once the finish is visible.
#else
        finishBlock.visible = true;
#endif

        finishBlock.solid = false;
    }
};

class Level {

public:
    static constexpr uint32_t BORDER_LINE_COLOR = 0x0000FF;
    static constexpr uint32_t LEVEL_LINE_COLOR = 0xFF0000;

    Level();
    void GenerateCollisionLines();
    void DrawLines(Surface* screen);

    const std::list<vec4>& GetCollisionLines() const; //GET 
    const std::list<LevelBlock>& GetLevelBlocks() const; //GET

private:
    void GenerateBorderBlock();
    void GenerateLevelBlocks();

    std::list<vec4> collisionLines = {};
    std::list<vec4> borderLines = {};
    std::list<vec4> levelLines = {};

    std::list<LevelBlock> levelBlocks = {};
};



