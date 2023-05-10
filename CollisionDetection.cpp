#include "CollisionDetection.h"
#include <algorithm>
#include <cmath>


bool CollisionDetection::lineRectangleCollision(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh)
{
	// Check for intersection between the line and each of the rectangle's edges
	bool left = lineLineIntersection(x1, y1, x2, y2, rx, ry, rx, ry + rh);
	bool right = lineLineIntersection(x1, y1, x2, y2, rx + rw, ry, rx + rw, ry + rh);
	bool top = lineLineIntersection(x1, y1, x2, y2, rx, ry, rx + rw, ry);
	bool bottom = lineLineIntersection(x1, y1, x2, y2, rx, ry + rh, rx + rw, ry + rh);

	// If any intersection is found, the line and rectangle collide
	return left || right || top || bottom;

}

bool CollisionDetection::lineLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	float denominator = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

	//might have to take this into account aswel since the bottom line could be parallel. Although if we calculate what line is closest after collision this would not matter.
	const float EPSILON = 1e-6;

	if (std::abs(denominator) < EPSILON) {
		return false; // The lines are parallel or coincident
	}

	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

	// If uA and uB are between 0-1, lines are intersecting
	return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
}

float CollisionDetection::pointLineDistance(float x, float y, float x1, float y1, float x2, float y2) {
	float lineLengthSquared = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	float t = ((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / lineLengthSquared;

	t = std::max(0.0f, std::min(1.0f, t));

	float nearestX = x1 + t * (x2 - x1);
	float nearestY = y1 + t * (y2 - y1);

	return sqrt((x - nearestX) * (x - nearestX) + (y - nearestY) * (y - nearestY));
}

RectangleSide CollisionDetection::findCollidingSide(float lineX1, float lineY1, float lineX2, float lineY2, float rectX, float rectY, float rectWidth, float rectHeight) {
	float leftMidX = rectX;
	float leftMidY = rectY + rectHeight / 2;

	float rightMidX = rectX + rectWidth;
	float rightMidY = leftMidY;

	float topMidX = rectX + rectWidth / 2;
	float topMidY = rectY;

	float bottomMidX = topMidX;
	float bottomMidY = rectY + rectHeight;

	float leftDist = pointLineDistance(leftMidX, leftMidY, lineX1, lineY1, lineX2, lineY2);
	float rightDist = pointLineDistance(rightMidX, rightMidY, lineX1, lineY1, lineX2, lineY2);
	float topDist = pointLineDistance(topMidX, topMidY, lineX1, lineY1, lineX2, lineY2);
	float bottomDist = pointLineDistance(bottomMidX, bottomMidY, lineX1, lineY1, lineX2, lineY2);

	float minDist = std::min({ leftDist, rightDist, topDist, bottomDist });

	if (minDist == bottomDist)
	{
		return RectangleSide::Bottom;
	}
	else if (minDist == topDist)
	{
		return RectangleSide::Top;
	}
	else if (minDist == rightDist)
	{
		return RectangleSide::Right;
	}
	else if (minDist == leftDist)
	{
		return RectangleSide::Left;
	}

	return RectangleSide::None;
}

