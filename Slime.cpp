#include "Slime.h"
#include "CollisionDetection.h"
#include <algorithm>    // std::max


	//The entire slime stuff should be moved to a class adventually 
	float slimeX = 100.0f, slimeY = 400.0f;
	float slimeSpeedX = 0.0f;
	float slimeSpeedY = 0.0f;
	float slimeJumpForce = 500.0f;
	float slimeMoveSpeed = 400.0f;
	int slimeFrame = 0;
	float gravity = 600.0f; //TODO refine this
	bool slimeGrounded = false;
	bool slimeMoving = false;
	bool sticky = false;

	//IDLE sprites i should be able to add this all together and loop through the frames. But for now this serves my purpose
	static Sprite slimeIdle1(std::make_shared<Surface>("assets/Individual_Slime/slime-idle-0.png"), 1);
	static Sprite slimeIdle2(std::make_shared<Surface>("assets/Individual_Slime/slime-idle-1.png"), 1);
	static Sprite slimeIdle3(std::make_shared<Surface>("assets/Individual_Slime/slime-idle-2.png"), 1);
	static Sprite slimeIdle4(std::make_shared<Surface>("assets/Individual_Slime/slime-idle-3.png"), 1);

	static Sprite slimeIdles[4] =
	{
		slimeIdle1,
		slimeIdle2,
		slimeIdle3,
		slimeIdle4
	};


	Slime::Slime(Surface* surface, Level& level, FinishCallback onFinish)
		:
		screen(surface),
		m_level(level)
	{
		m_SlimeDirection = Direction::RIGHT;
		
		slimeX = ((float)ScreenWidth / 2);
		slimeY = ((float)ScreenHeight - 200);

		m_initialSpeedXWhenJumping = 0.0f; //Set this to 0 on initialize
	}

	void Slime::Draw(float deltaTime)
	{

#ifdef NDEBUG
		
#else
		screen->Box((int)slimeX, (int)slimeY, (int)(slimeX + slimeIdles[slimeFrame].GetWidth()), (int)(slimeY + slimeIdles[slimeFrame].GetHeight()), 0x00FF00);
#endif

		float deltaTimeInSeconds = deltaTime / 1000.0f;

		m_stickyEffectTimer -= (m_stickyEffectTimer -= deltaTimeInSeconds);


		if (slimeGrounded && !slimeMoving)
		{
			slimeSpeedX = 0;
		}

		if (sticky)
		{
			slimeSpeedY /= static_cast<float>(pow((double)2.5f, (double)deltaTimeInSeconds)); // Divide slimeSpeedY by 2 every second
		}

		// Update slime posistion
		slimeX += slimeSpeedX * deltaTimeInSeconds;
		slimeY += slimeSpeedY * deltaTimeInSeconds;

		//UpdateIsGrounded();
		UpdateSlimeSpeedY(deltaTime);
		UpdateClimbingSticky(deltaTime);

		if (m_SlimeDirection == Direction::LEFT)
		{
			slimeIdles[slimeFrame].Draw(screen, (int)slimeX, (int)slimeY);
		}
		else if (m_SlimeDirection == Direction::RIGHT)
		{
			slimeIdles[slimeFrame].DrawFlipped(screen, (int)slimeX, (int)slimeY);
		}

		UpdateSprite(deltaTime);

		bool collidingFromTop = false;
		bool collidingFromLeft = false;
		bool collidingFromRight = false;

		CheckCollision(collidingFromTop, collidingFromLeft, collidingFromRight);

		if (collidingFromLeft || collidingFromRight) {
			if (slimeMoving && sticky) {
				float verticalClimbSpeed = 100.0f;
				slimeY -= verticalClimbSpeed * deltaTimeInSeconds;
			}
		}

		//Reset stick effect timer
		if (!sticky || !collidingFromTop)
		{
			m_stickyEffectTimer = 0.0f;
		}
	}

	void Slime::UpdateSprite(float deltaTime)
	{
		float deltaTimeInSeconds = deltaTime / 1000.0f;

		m_timeSinceLastFrameUpate += deltaTimeInSeconds;

		if (m_timeSinceLastFrameUpate >= 0.25f) // 1.0f represents one second
		{
			slimeFrame = (slimeFrame == sizeof(slimeIdles) / sizeof(Sprite)) ? 0 : slimeFrame = slimeFrame + 1;
			m_timeSinceLastFrameUpate -= 0.25f;
		}
	}


	void Slime::Jump()
	{
		if (!slimeGrounded) 
		{ 
			if (!sticky) { return; }

			//slimeSpeedY = (-slimeJumpForce / 8);
		}
		else 
		{
			printf("Slime jump!\n");

			m_initialSpeedXWhenJumping = slimeSpeedX;
			slimeGrounded = false;
			
			if (sticky)
			{
				slimeSpeedY = -slimeJumpForce / 2;
			}
			else
			{
				slimeSpeedY = -slimeJumpForce;
			}
		}
	}

	

	void Slime::CheckCollision(bool& collidingFromTop, bool& collidingFromLeft, bool& collidingFromRight)
	{
		//Create a instance of collisionDetection
		CollisionDetection collisionDetection;

		bool collision = false;

		for each (LevelBlock block in m_level.GetLevelBlocks())
		{
			for each (vec4 line in block.lines)
			{
				collision = collisionDetection.lineRectangleCollision(line.x, line.y, line.z, line.w, slimeX, (slimeY + 1), (float)slimeIdles[slimeFrame].GetWidth(), (float)slimeIdles[slimeFrame].GetHeight());

				if (collision)
				{
					printf("Collision!\n");



					RectangleSide rectangleSide = collisionDetection.findCollidingSide(line.x, line.y, line.z, line.w, slimeX, slimeY, (float)slimeIdles[slimeFrame].GetWidth(), (float)slimeIdles[slimeFrame].GetHeight());

					HandleCollision(block, rectangleSide, line, collidingFromTop, collidingFromLeft, collidingFromRight);

					break;
				}
			}
		}

		if (!collision)
		{
			printf("No Collision!\n");


			if (m_stickyEffectTimer <= 0.0f) { sticky = false; }
		}
	}

	void Slime::HandleCollision(LevelBlock block, RectangleSide side, vec4 line, bool& collidingFromTop, bool& collidingFromLeft, bool& collidingFromRight)
	{
		float speedMod = 0.7f;
		const float maxStickyEffectTime = 3000.0f; // 3000 milliseconds (3 seconds)
		sticky = false;
		m_climbingSticky = false;

		//Set values based on what type of block we collided with
		switch (block.type)
		{
		case LevelBlock::BlockType::regular:
			speedMod = 0.7f;
			break;

		case LevelBlock::BlockType::sticky:
			printf("Sticky!\n");
			sticky = true;
			m_stickyEffectTimer = 1.5f;
			break;

		case LevelBlock::BlockType::bouncy:
			break;

		case LevelBlock::BlockType::finish:
			
			if (m_onFinish) { m_onFinish(); }

			return;
			break;
		}

		//Handle collision based on what side the slime hit the wall
		switch (side)
		{
		case RectangleSide::Top:
			printf("Collision Top!\n");
			collidingFromTop = true;

			if (sticky)
			{
				m_stickyEffectTimer = 0.0f;
				slimeSpeedY = 0.0f;
			}

			if (m_stickyEffectTimer >= maxStickyEffectTime)
			{
				slimeSpeedY = slimeJumpForce / 2;
			}

			slimeY = ((slimeY - line.y) < (slimeY - line.w)) ? slimeY = line.y : slimeY = line.w;
			slimeSpeedY = (sticky) ? slimeSpeedY = 0 : slimeSpeedY = -(slimeSpeedY * (speedMod - 0.3f));
			break;

		case RectangleSide::Right:
			printf("Collision Right!\n");
			collidingFromRight = true;

			if (sticky)
			{
				m_climbingSticky = true;
				slimeY -= 1.0f; // Add a small upward offset to prevent bouncing
			}

			slimeX = ((slimeX - line.x) > (slimeX - line.z)) ? slimeX = ((line.x - slimeIdles[slimeFrame].GetWidth()) - 1) : slimeX = ((line.z - slimeIdles[slimeFrame].GetWidth()) - 1);
			slimeSpeedX = (slimeGrounded || sticky) ? slimeSpeedX = 0 : slimeSpeedX = -(slimeSpeedX * speedMod);

			if (!slimeGrounded) { m_SlimeDirection = Direction::LEFT; }
			break;

		case RectangleSide::Bottom:
			if (!slimeGrounded)
			{
				printf("Collision Bottom!\n");
			}

			slimeY = ((line.y - slimeIdles[slimeFrame].GetHeight()));
			slimeGrounded = true;
			slimeSpeedY = 0;
			break;

		case RectangleSide::Left:
			printf("Collision Left!\n");
			collidingFromLeft = true;

			if (sticky)
			{
				m_climbingSticky = true;
				slimeY -= 1.0f; // Add a small upward offset to prevent bouncing
			}

			slimeX = ((slimeX - line.x) < (slimeX - line.z)) ? slimeX = (line.x + 1) : slimeX = (line.z + 1);
			slimeSpeedX = (slimeGrounded || sticky) ? slimeSpeedX = 0 : slimeSpeedX = -(slimeSpeedX * speedMod);

			if (!slimeGrounded) { m_SlimeDirection = Direction::RIGHT; }
			break;
		}
	}

	void Slime::Move(Direction direction)
	{

		slimeMoving = true;

		float adjustSlimeSpeed = slimeMoveSpeed;
		float maxAirSpeedChange = 0.5f * slimeMoveSpeed; // To limit the ammount the player can change in speed while jumping.
		float airControlFactor = 0.2f;

		m_SlimeDirection = direction;

		if (direction == Direction::LEFT)
		{
			printf("Slime Direction Left!\n");

			if (slimeGrounded) {
				slimeSpeedX = -std::max(adjustSlimeSpeed, (slimeSpeedX -= adjustSlimeSpeed)); // On the ground we can do full speed
			}
			else {
				float targetSpeed = m_initialSpeedXWhenJumping - adjustSlimeSpeed * airControlFactor;
				if (slimeSpeedX > targetSpeed) {
					slimeSpeedX = std::max(targetSpeed, slimeSpeedX - adjustSlimeSpeed * airControlFactor);
				}

				slimeSpeedX = std::max(slimeSpeedX, -maxAirSpeedChange); // Ensure the character doesn't speed up too much after bouncing off a wall

			}
		}

		if (direction == Direction::RIGHT)
		{
			printf("Slime Direction Right!\n");

			if (slimeGrounded) {
				slimeSpeedX = std::min(adjustSlimeSpeed, (slimeSpeedX += adjustSlimeSpeed)); // On the ground we can do full speed
			}
			else {
				float targetSpeed = m_initialSpeedXWhenJumping + adjustSlimeSpeed * airControlFactor;
				if (slimeSpeedX < targetSpeed) {
					slimeSpeedX = std::min(targetSpeed, slimeSpeedX + adjustSlimeSpeed * airControlFactor);
				}

				slimeSpeedX = std::min(slimeSpeedX, maxAirSpeedChange); // Ensure the character doesn't speed up too much after bouncing off a wall
			}
		}

	}

	void Slime::UpdateSlimeSpeedY(float deltaTime)
	{
		float deltaTimeInSeconds = deltaTime / 1000.0f;

		if (sticky)
		{
			slimeSpeedY /= static_cast<float>(pow(2.5f, deltaTimeInSeconds)); // Divide slimeSpeedY by 2 every second
		}
		else
		{
			slimeSpeedY += gravity * deltaTimeInSeconds;
		}
	}

	void Slime::UpdateClimbingSticky(float deltaTime)
	{
		float deltaTimeInSeconds = deltaTime / 1000.0f;
		float climbingSpeed = 200.0f;

		if (m_climbingSticky)
		{
			slimeY -= climbingSpeed * deltaTimeInSeconds;
		}
	}

	void Slime::KeyDown(int key)
	{
		switch (key)
		{
		case 44: //JUMP
			Jump();
			break;
		case 4: case 80: //4 = A 80 = Left Arrow LEFT
			Move(Direction::LEFT);
			break;
		case 7: case 79: //7 = D 79 = Right Arrow RIGHT
			Move(Direction::RIGHT);
			break;

		case 26: case 82: //26 = W 82 = Up Arrow UP

			break;

		case 22: case 81: //22 = S 81 = Down Arrow DOWN

			break;
		}
	}

	void Slime::KeyUp(int key)
	{
		switch (key)
		{
		case 44: //JUMP
			break;
		case 4: case 80: //4 = A 80 = Left Arrow LEFT
			slimeMoving = false;
			if (slimeGrounded)
			{
				slimeSpeedX = 0;
			}

			//slimeSpeedX = 0;
			break;
		case 7: case 79: //7 = D 79 = Right Arrow RIGHT
			slimeMoving = false;
			if (slimeGrounded)
			{
				slimeSpeedX = 0;
			}
			break;

		case 26: case 82: //26 = W 82 = Up Arrow UP

			break;

		case 22: case 81: //22 = S 81 = Down Arrow DOWN

			break;
		}
	}

	//
	void Slime::UpdateIsGrounded()
	{
		slimeGrounded = false;
		CollisionDetection collisionDetection;

		for (const LevelBlock& block : m_level.GetLevelBlocks())
		{
			for (const vec4& line : block.lines)
			{
				if (collisionDetection.lineRectangleCollision(
					line.x, line.y, line.z, line.w,
					slimeX, slimeY + 1,
					(float)slimeIdles[slimeFrame].GetWidth(),
					(float)slimeIdles[slimeFrame].GetHeight()))
				{
					if (collisionDetection.findCollidingSide(line.x, line.y, line.z, line.w, slimeX, slimeY, (float)slimeIdles[slimeFrame].GetWidth(), (float)slimeIdles[slimeFrame].GetHeight()) == RectangleSide::Bottom)
					{
						slimeGrounded = true;
					}

					return;
				}
			}
		}
	}
