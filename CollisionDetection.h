#pragma once

enum class RectangleSide {
	Left, Right, Top, Bottom, None
};

class CollisionDetection
{
public:
	bool lineRectangleCollision(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh); //Calculate if a rectangle and line are colliding
	bool lineLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4); //Calculate if two lines are intersecting
	float pointLineDistance(float x, float y, float x1, float y1, float x2, float y2); //Calculate the distance between the line
	RectangleSide findCollidingSide(float lineX1, float lineY1, float lineX2, float lineY2, float rectX, float rectY, float rectWidth, float rectHeight); //Return the side of the rectangle that is closest
private:


};

