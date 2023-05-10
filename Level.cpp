#include "Level.h"
#include "game.h"
#include "template.h"

#include "surface.h"
#include <xpolymorphic_allocator.h>
#include <algorithm>
#include <cmath>
using namespace Tmpl8;

Level::Level() {
    GenerateCollisionLines();
}

void Level::GenerateCollisionLines() {
    GenerateBorderBlock();
    GenerateLevelBlocks();

    collisionLines.insert(collisionLines.end(), borderLines.begin(), borderLines.end());
    collisionLines.insert(collisionLines.end(), levelLines.begin(), levelLines.end());
}

void Level::GenerateBorderBlock() {

	LevelBlock windowBorder(LevelBlock(1.0f, 1.0f, (float)ScreenWidth, (float)ScreenHeight, LevelBlock::BlockType::regular));

	// if debug true else false
#ifdef NDEBUG
	windowBorder.visible = false;
#else
	windowBorder.visible = true;
#endif

	levelBlocks.push_back(windowBorder);
}

void Level::GenerateLevelBlocks() {

	//FLOOR
	levelBlocks.push_back(LevelBlock(0, (float)ScreenHeight - 20, (float)ScreenWidth, 5.0f, LevelBlock::BlockType::regular));



	//levelBlocks.push_back(LevelBlock(400, (float)ScreenHeight - 240, 400, 5.0f, LevelBlock::BlockType::sticky));



	//PLATFORMS
	levelBlocks.push_back(LevelBlock(200, (float)ScreenHeight - 150, 300, 20, LevelBlock::BlockType::regular));
	levelBlocks.push_back(LevelBlock(600, (float)ScreenHeight - 400, 200, 20, LevelBlock::BlockType::regular));
	levelBlocks.push_back(LevelBlock(850, (float)ScreenHeight - 650, 60, 200, LevelBlock::BlockType::sticky));
	levelBlocks.push_back(LevelBlock(300, (float)ScreenHeight - 600, 200, 20, LevelBlock::BlockType::regular));

	levelBlocks.push_back(LevelBlock(500, (float)ScreenHeight - 300, 600, 20, LevelBlock::BlockType::regular));
	levelBlocks.push_back(LevelBlock(1100, (float)ScreenHeight - 800, 30, 520, LevelBlock::BlockType::regular));

	levelBlocks.push_back(LevelBlock(1100, (float)ScreenHeight - 210, 30, 190, LevelBlock::BlockType::regular));
	levelBlocks.push_back(LevelBlock(1100, (float)ScreenHeight - 230, 130, 20, LevelBlock::BlockType::regular));
	levelBlocks.push_back(LevelBlock(1230, (float)ScreenHeight - 230, 20, 210, LevelBlock::BlockType::sticky));

	levelBlocks.push_back(LevelBlock(100, (float)ScreenHeight - 800, 300, 20, LevelBlock::BlockType::sticky));
	levelBlocks.push_back(LevelBlock(650, (float)ScreenHeight - 840, 300, 20, LevelBlock::BlockType::regular));

	levelBlocks.push_back(LevelBlock(1350, (float)ScreenHeight - 640, 300, 20, LevelBlock::BlockType::regular));

	levelBlocks.push_back(LevelBlock((float)ScreenWidth - 330, 0, 30, 320, LevelBlock::BlockType::regular));
	levelBlocks.push_back(LevelBlock((float)ScreenWidth - 30, 0, 30, 600, LevelBlock::BlockType::sticky));
	levelBlocks.push_back(LevelBlock((float)ScreenWidth - 80, 600, 80, 30, LevelBlock::BlockType::regular));

	//FINISH
	LevelBlock baseBlock((float)ScreenWidth - 300, 300, 200, 20, LevelBlock::BlockType::regular);
	LevelBlock finishBlock(baseBlock.x + (baseBlock.width / 2), baseBlock.y - 100, 30, 100, LevelBlock::BlockType::finish);

	Finish finish(baseBlock, finishBlock);

	levelBlocks.push_back(finish.baseBlock);
	levelBlocks.push_back(finish.finishBlock);

}

void Level::DrawLines(Surface* screen) {
    
	for each (LevelBlock block in levelBlocks)
	{
		block.Draw(screen);
		//block.DrawFilled(screen);
	}
}

const std::list<vec4>& Level::GetCollisionLines() const 
{
	return collisionLines;
}

const std::list<LevelBlock>& Level::GetLevelBlocks() const
{
	return levelBlocks;
}

const std::map<LevelBlock::BlockType, uint32_t> LevelBlock::blockTypeToColor = {
	{LevelBlock::BlockType::regular, 0xFF0000}, // red
	{LevelBlock::BlockType::bouncy, 0x00FF00},  // green
	{LevelBlock::BlockType::sticky, 0x0000FF},   // blue
	{LevelBlock::BlockType::finish, 0xFBFAF5} //Softer white
};


