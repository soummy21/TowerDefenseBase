#include "Game.h"

#include <SFML/Graphics.hpp>
#include "Helper.h"
#include <algorithm>
#include <vector>

Game::Game():
    gameWindow(sf::VideoMode(1920, 1080), "Game Window"),
    gameMode(Play),
    requestedMovementDirection(0.0f, 0.0f),
    playerSpeed(100.0f),
    scrollWheelInput(None),
    tileOptionIndex(0),
    player(Entity::PhysicsProperties::Type::Dynamic),
    enemyBlueprint(Entity::PhysicsProperties::Type::Dynamic),
	weapon(Entity::PhysicsProperties::Type::Static)
{
    const float imageSize = 16.0f;
    const float tileMapSize = 64.0f;
    const float spriteOrigin = imageSize / 2.0f;
    const float playerSize = 10.0f;
	const float weaponSize = 5.0f;
    const float enemySize = 5.0f;
    const sf::Vector2f windowOrigin = sf::Vector2f(960.0f, 540.0f);

    //Setup Player Sprite
    playerTexture.loadFromFile("Images/Player.png");
    player.SetTexture(playerTexture);
    player.SetScale(sf::Vector2f(playerSize, playerSize));
    player.SetOrigin(sf::Vector2f(spriteOrigin, spriteOrigin));
	player.SetCircle((playerSize * imageSize) / 2);   
    player.AddEntityToIgnoreForCollision(&weapon);

    //Setup Enemy Sprite
    enemyTexture.loadFromFile("Images/Enemy.png");
    enemyBlueprint.SetTexture(enemyTexture);
    enemyBlueprint.SetScale(sf::Vector2f(enemySize, enemySize));
    enemyBlueprint.SetOrigin(sf::Vector2f(spriteOrigin, spriteOrigin));
    enemyBlueprint.SetPosition(windowOrigin);
	enemyBlueprint.SetCircle((enemySize * imageSize) / 2);

    //Setup Weapon Sprite
    weaponTexture.loadFromFile("Images/Weapon.png");
    weapon.SetTexture(weaponTexture);
    weapon.SetScale(sf::Vector2f(weaponSize, weaponSize));
    weapon.SetOrigin(sf::Vector2f(spriteOrigin, spriteOrigin));
	weapon.SetCircle((weaponSize * imageSize) / 2);

    defaultFont.loadFromFile("Fonts/BAUHS93.TTF");

    gameModeText.setPosition(sf::Vector2f(1600.0f, 30.0f));
    gameModeText.setString("Mode: Play");
    gameModeText.setFont(defaultFont);

    tileMapTexture.loadFromFile("Images/TileMap.png");

    //Read the TileMap texture and add the tiles to tile options
    int noOfTiles = static_cast<int>( tileMapSize / imageSize);
    for (int y = 0; y < noOfTiles; y++)
    {
        for (int x = 0; x < noOfTiles; x++)
        {
            sf::Sprite tile; 
            tile.setTexture(tileMapTexture);
            tile.setTextureRect(sf::IntRect(x * imageSize, y * imageSize, imageSize, imageSize));
            tile.setScale(sf::Vector2f(playerSize, playerSize));
            tile.setOrigin(sf::Vector2f(spriteOrigin, spriteOrigin));

			TileOption::TileType tileType = TileOption::TileType::Null;

            if (y == 0)
            {
				tileType = TileOption::TileType::Aesthetic;
			}
            else
            {
                if(y == 1)
                {
                    if (x == 0)
                    {
                        tileType = TileOption::TileType::Spawn;
                    }
                    else if (x == 1)
                    {
                        tileType = TileOption::TileType::End;
                    }
                    else if (x == 2)
                    {
                        tileType = TileOption::TileType::Path;
                    }
                }

            }

            tileOptions.emplace_back(tileType);
			TileOption& tileOption = tileOptions.back();
            tileOption.SetSprite(tile);
        }
    }

}

Game::~Game()
{
}

void Game::Tick()
{
    sf::Clock clock;
    while (gameWindow.isOpen())
    {
        deltaTime = clock.restart();

        HandleInput();

        switch (gameMode)
        {
        case Game::Play: UpdatePlayMode();
            break;
        case Game::LevelEditor: UpdateLevelEditorMode();
            break;
        default:
            break;
        }

        Draw();
    }

}

void Game::UpdatePlayMode()
{
    player.SetVelocity(requestedMovementDirection * playerSpeed);

 //   ////Weapon movement relative to player
 //   sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
 //   sf::Vector2f playerToMouse = (sf::Vector2f)mousePosition - player.GetPosition();
 //   sf::Vector2f normalisedPlayerToMouse = Math::NormalizeVector(playerToMouse);
	//float playerHalfSize = player.GetPhysicsProperties().radius;
	//float weaponHalfSize = weapon.GetPhysicsProperties().radius;
 //   sf::Vector2f weaponOffsetToPlayer = normalisedPlayerToMouse * (playerHalfSize + weaponHalfSize);
 //   sf::Vector2f weaponPosition = player.GetPosition() + weaponOffsetToPlayer;
 //   weapon.SetPosition(weaponPosition);


	//Spawn enemies and set a random path for them to follow
    const int noOfEnemies = 4;
    if (!spawnTiles.empty() && !paths.empty())
    {
        enemyBlueprint.SetPosition(spawnTiles[0].GetPosition());

        if (enemies.size() < noOfEnemies)
        {
            enemies.emplace_back(enemyBlueprint);
            Entity& newEnemy = enemies.back();
			newEnemy.SetPathIndex(std::rand() % paths.size());

        }
    }

    float enemySpeed = 70.0f;

    //Set Velocity for the enemies 
    for (int i = enemies.size() - 1; i >= 0; --i)
    {
		Entity& enemy = enemies[i];
		Path& path = paths[enemy.GetPathIndex()];

		PathTile* closestPathTile = nullptr;
		float closestDistance = std::numeric_limits<float>::max();

		//Find the closest path tile to the enemy
		for (PathTile& pathTile : path)
		{
			sf::Vector2f enemyToPathTile = pathTile.currentTile->GetPosition() - enemy.GetPosition();
			float distance = Math::GetVectorLength(enemyToPathTile);

			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestPathTile = &pathTile;
			}
		}
		//Find the next tile in the path
		const Entity* nextTile = closestPathTile->nextTile;

        if (!nextTile)
        {
			nextTile = closestPathTile->currentTile;
        }

		//If the enemy reaches the end tile, delete it
		if (nextTile->GetClosestCoordinates() == endTiles[0].GetClosestCoordinates())
		{
            if (closestDistance < 40.0f)
            {
                enemies.erase(enemies.begin() + i);
                continue;
            }

		}
        
        //Set the velocity of the enemy
		sf::Vector2f enemyToNextTile = nextTile->GetPosition() - enemy.GetPosition();
		sf::Vector2f normalisedEnemyToNextTile = Math::NormalizeVector(enemyToNextTile);
		enemy.SetVelocity(normalisedEnemyToNextTile * enemySpeed);
    }

    UpdatePhysics();

}

void Game::UpdateLevelEditorMode()
{
	enemies.clear();
}

void Game::UpdatePhysics()
{
    std::vector<Entity*> allEntities;
    allEntities.push_back(&player);
    allEntities.push_back(&weapon);

    for (Entity& enemy : enemies)
    {
		allEntities.push_back(&enemy);
    }

	for (Entity& tile : collisionTiles)
	{
		allEntities.push_back(&tile);
	}

    for (Entity* entity : allEntities)
    {
        if (entity->GetPhysicsProperties().type == Entity::PhysicsProperties::Type::Dynamic)
        {
            entity->Move(entity->GetPhysicsProperties().velocity * deltaTime.asSeconds());

            //Check for collisions
            for (Entity* otherEntity : allEntities)
            {
                if (entity == otherEntity)
                    continue;

				if (entity->ShouldIgnoreEntityForCollision(otherEntity))
					continue;

                ProcessCollision(*entity, *otherEntity);
            }
        }
    }
}

//Collision math from https://www.jeffreythompson.org/collision-detection/
void Game::ProcessCollision(Entity& entity, Entity& otherEntity)
{
    //Circle-Circle collision detection
    if (entity.GetPhysicsProperties().shape == Entity::PhysicsProperties::Shape::Circle)
    {
        if (otherEntity.GetPhysicsProperties().shape == Entity::PhysicsProperties::Shape::Circle)
        {
            const sf::Vector2f entityToOtherEntity = otherEntity.GetPosition() - entity.GetPosition();
            const float distanceBetweenEntities = Math::GetVectorLength(entityToOtherEntity);
            const float sumOfRadius = entity.GetPhysicsProperties().radius + otherEntity.GetPhysicsProperties().radius;

            if (distanceBetweenEntities < sumOfRadius)
            {
                const bool isOtherEntityDynamic = otherEntity.GetPhysicsProperties().type == Entity::PhysicsProperties::Type::Dynamic;
                if (!isOtherEntityDynamic)
                {
                    const sf::Vector2f direction = -Math::NormalizeVector(entityToOtherEntity);
                    const float distance = sumOfRadius - distanceBetweenEntities;
                    entity.Move(direction * distance);
                }
                else
                {
                    const sf::Vector2f direction = Math::NormalizeVector(entityToOtherEntity);
                    const float distance = (sumOfRadius - distanceBetweenEntities) * 0.5f;
                    entity.Move(-direction * distance);
                    otherEntity.Move(direction * distance);
                }
            }
        }
        //Circle-Rectangle Collision Dectection
        else if(otherEntity.GetPhysicsProperties().shape == Entity::PhysicsProperties::Shape::Rectangle)
        {
            //Get the nearest position of x and y on the rectangle 
            float nearestPositionX = std::max(otherEntity.GetPosition().x - 0.5f * otherEntity.GetPhysicsProperties().width, 
                                              std::min(entity.GetPosition().x, 
                                                       otherEntity.GetPosition().x + 0.5f * otherEntity.GetPhysicsProperties().width));
            float nearestPositionY = std::max(otherEntity.GetPosition().y - 0.5f * otherEntity.GetPhysicsProperties().height, 
                                              std::min(entity.GetPosition().y, 
                                                       otherEntity.GetPosition().y + 0.5f * otherEntity.GetPhysicsProperties().height));
            //Vector from center to that point 
            sf::Vector2f nearestVectorOnRectangle(nearestPositionX, nearestPositionY);
            sf::Vector2f entityToOtherEntityVector = nearestVectorOnRectangle - entity.GetPosition();
            //Get the distance 
            float distance = Math::GetVectorLength(entityToOtherEntityVector);
            if (distance < entity.GetPhysicsProperties().radius)
            {
                const sf::Vector2f normalisedEntityToOtherEntity = Math::NormalizeVector(entityToOtherEntityVector);
                const bool isOtherEntityDynamic = otherEntity.GetPhysicsProperties().type == Entity::PhysicsProperties::Type::Dynamic;
                if (!isOtherEntityDynamic)
                {
                    entity.Move(-normalisedEntityToOtherEntity * (entity.GetPhysicsProperties().radius - distance));
                }
                else
                {
                    float movement = (entity.GetPhysicsProperties().radius - distance) * 0.5f;
                    entity.Move(-normalisedEntityToOtherEntity * movement);
                    otherEntity.Move(normalisedEntityToOtherEntity * movement);
                }
            }



        }

    }
	//Rectangle-Circle Collision Detection
    else if (entity.GetPhysicsProperties().shape == Entity::PhysicsProperties::Shape::Rectangle)
    {
        if (otherEntity.GetPhysicsProperties().shape == Entity::PhysicsProperties::Shape::Circle)
        {
            //Get the nearest position of x and y on the rectangle 
            float nearestPositionX = std::max(entity.GetPosition().x - 0.5f * entity.GetPhysicsProperties().width, 
                                              std::min(otherEntity.GetPosition().x, 
                                                       entity.GetPosition().x + 0.5f * entity.GetPhysicsProperties().width));
            float nearestPositionY = std::max(entity.GetPosition().y - 0.5f * entity.GetPhysicsProperties().height, 
                                              std::min(otherEntity.GetPosition().y, 
                                                       entity.GetPosition().y + 0.5f * entity.GetPhysicsProperties().height));
            //Vector from center to that point 
            sf::Vector2f nearestVectorOnRectangle(nearestPositionX, nearestPositionY);
            sf::Vector2f otherEntityToEntityVector = nearestVectorOnRectangle - otherEntity.GetPosition();
            //Get the distance 
            float distance = Math::GetVectorLength(otherEntityToEntityVector);
            if (distance < otherEntity.GetPhysicsProperties().radius)
            {
                const sf::Vector2f normalisedEntityToOtherEntity = Math::NormalizeVector(otherEntityToEntityVector);
                const bool isOtherEntityDynamic = otherEntity.GetPhysicsProperties().type == Entity::PhysicsProperties::Type::Dynamic;
                if (!isOtherEntityDynamic)
                {
                    entity.Move(normalisedEntityToOtherEntity * (otherEntity.GetPhysicsProperties().radius - distance));
                }
                else
                {
                    float movement = (entity.GetPhysicsProperties().radius - distance) * 0.5f;
                    entity.Move(normalisedEntityToOtherEntity * movement);
                    otherEntity.Move(-normalisedEntityToOtherEntity * movement);
                }
            }
        }
		//Rectangle-Rectangle Collision Detection
        else if (otherEntity.GetPhysicsProperties().shape == Entity::PhysicsProperties::Shape::Rectangle)
        {
            float distanceX = std::abs(entity.GetPosition().x - otherEntity.GetPosition().x);
            float distanceY = std::abs(entity.GetPosition().y - otherEntity.GetPosition().y);

            float overlapX = 0.5f * (entity.GetPhysicsProperties().width + otherEntity.GetPhysicsProperties().width) - distanceX;
            float overlapY = 0.5f * (entity.GetPhysicsProperties().height + otherEntity.GetPhysicsProperties().height) - distanceY;

            if (overlapX > 0 && overlapY > 0)
            {
				const bool isOtherEntityDynamic = otherEntity.GetPhysicsProperties().type == Entity::PhysicsProperties::Type::Dynamic;

				//Figures out whether the rectangle collides on the x or y axis 
                if (overlapX < overlapY)
                {
					//If the entity is to the left of the other entity
                    if (entity.GetPosition().x < otherEntity.GetPosition().x)
                    {
                        if (!isOtherEntityDynamic)
                        {
							entity.Move(sf::Vector2f(-overlapX, 0.0f));
                        }
                        else
                        {
                            entity.Move(sf::Vector2f(-overlapX * 0.5f, 0.0f));
                            otherEntity.Move(sf::Vector2f(overlapX * 0.5f, 0.0f));
                        }

                    }
					//If the entity is to the right of the other entity
                    else
                    {

                        if (!isOtherEntityDynamic)
                        {
                            entity.Move(sf::Vector2f(overlapX, 0.0f));
                        }
                        else
                        {
                            entity.Move(sf::Vector2f(overlapX * 0.5f, 0.0f));
                            otherEntity.Move(sf::Vector2f(-overlapX * 0.5f, 0.0f));
                        }
      
                    }
                }
                else
                {
					//If the entity is above the other entity
                    if (entity.GetPosition().y < otherEntity.GetPosition().y)
                    {
                        if (!isOtherEntityDynamic)
                        {
							entity.Move(sf::Vector2f(0.0f, -overlapY));
						}
                        else
                        {
                            entity.Move(sf::Vector2f(0.0f, -overlapY * 0.5f));
                            otherEntity.Move(sf::Vector2f(0.0f, overlapY * 0.5f));
                        }
                    }
					//If the entity is below the other entity
                    else
                    {
                        if (!isOtherEntityDynamic)
                        {
                            entity.Move(sf::Vector2f(0.0f, overlapY));
                        }
                        else
                        {
							entity.Move(sf::Vector2f(0.0f, overlapY * 0.5f));
							otherEntity.Move(sf::Vector2f(0.0f, -overlapY * 0.5f));
                        }
                    }
                }
            }
        }

    }
}

void Game::Draw()
{
    gameWindow.clear();

    //Draw the tiles
	for (const Entity& tile : collisionTiles)
	{
		gameWindow.draw(tile);
	}

	for (const Entity& enemy : enemies)
	{
		gameWindow.draw(enemy);
	}

    //Draw the sprites 
    gameWindow.draw(player);
    //gameWindow.draw(weapon);
    gameWindow.draw(gameModeText);

    switch (gameMode)
    {
    case Game::Play: 
        break;
    case Game::LevelEditor:
        DrawLevelEditor();
        break;
    default:
        break;
    }

    gameWindow.display();
}

void Game::DrawLevelEditor()
{
    sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(gameWindow);
    tileOptions[tileOptionIndex].setPosition(mousePosition);

    for (const Entity& tile : spawnTiles)
    {
        gameWindow.draw(tile);
    }

    for (const Entity& tile : endTiles)
    {
        gameWindow.draw(tile);
    }

    for (const Entity& tile : pathTiles)
    {
        gameWindow.draw(tile);
    }

    gameWindow.draw(tileOptions[tileOptionIndex]);
}

void Game::HandleInput()
{
    static bool WasPressedLastUpdate = false;
    //Change Game Mode
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
    {
        //On Player Click T
        if (!WasPressedLastUpdate)
        {
            if (gameMode == Play)
            {
                gameMode = LevelEditor;
                gameModeText.setString("Mode: Level Editor");
            }
            else
            {
                gameMode = Play;
                gameModeText.setString("Mode: Play");
            }
        }
            

        WasPressedLastUpdate = true;
        //keeps the bool true when player keeps clicking T
    }
    else
    {
        WasPressedLastUpdate = false; //keeps the bool false when player isn't clicking T
    }


    sf::Event event;
    scrollWheelInput = None;
    while (gameWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            gameWindow.close();
            break;
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                if (event.mouseWheelScroll.delta > 0.0f)
                {
                    scrollWheelInput = ScrollUp;
                }
                else
                {
                    scrollWheelInput = ScrollDown;
                }
            }
            break;
        default:
            break;
        }

    }

    switch (gameMode)
    {
        case Game::Play: 
            HandlePlayerInput();
            break;
        case Game::LevelEditor: 
            HandleLevelEditorInput();
            break;
        default:
            break;
    }
}

void Game::HandlePlayerInput()
{
    //Player Movement
    sf::Vector2f rightMovement(1.0f, 0.0f);
    sf::Vector2f leftMovement(-1.0f, 0.0f);
    sf::Vector2f upMovement(0.0f, -1.0f);
    sf::Vector2f downMovement(0.0f, 1.0f);

    requestedMovementDirection = sf::Vector2f(0.0f, 0.0f);

	//Check if the player is pressing the key W and move in the up direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        requestedMovementDirection += upMovement;
    }

	//Check if the player is pressing the key A and move in the left direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        requestedMovementDirection += leftMovement;
    }

	//Check if the player is pressing the key S and move in the down direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        requestedMovementDirection += downMovement;
    }

	//Check if the player is pressing the key D and move in the right direction
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        requestedMovementDirection += rightMovement;
    }

    requestedMovementDirection = Math::NormalizeVector(requestedMovementDirection);
}

void Game::HandleLevelEditorInput()
{
	//Scroll up and down to change the tile option
    if (scrollWheelInput == ScrollUp)
    {
        tileOptionIndex++; 
        if (tileOptionIndex >= tileOptions.size())
            tileOptionIndex = 0;
    }
    else if(scrollWheelInput == ScrollDown)
    {
        tileOptionIndex--; 
        if (tileOptionIndex < 0)
            tileOptionIndex = tileOptions.size() - 1;
    }

	//Check if the player is pressing the left mouse button and create a tile during edit mode
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(gameWindow);
        CreateTileAtPosition(mousePosition);
    }

	//Check if the player is pressing the right mouse button and delete a tile during edit mode
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(gameWindow);
        DeleteTileAtPosition(mousePosition);
    }
}


void Game::ConstructPath()
{
    paths.clear();

	if (spawnTiles.empty() || endTiles.empty())
		return;

	//Create a path from the spawn tile to the end tile
    Path path; 
    path.emplace_back();
    PathTile& startTile = path.back();
    startTile.currentTile = &spawnTiles[0];

	sf::Vector2i endCoords = endTiles[0].GetClosestCoordinates();

	VisitNeighbourInPath(path, endCoords);
}

//Create Tile at Position during Level Editor Mode
void Game::CreateTileAtPosition(const sf::Vector2f& position)
{
	TileOption::TileType tileType = tileOptions[tileOptionIndex].GetTileType();
    if (tileType == TileOption::TileType::Null)
        return;

    std::vector<Entity>& tilesList = GetTileList(tileType);

    //Only allows one spawn or end tile
    if (tileType == TileOption::TileType::Spawn || tileType == TileOption::TileType::End)
    {
        tilesList.clear();
    }

    int x = position.x / 160;
    int y = position.y / 160;

    sf::Sprite sprite = tileOptions[tileOptionIndex].GetSprite();
    sprite.setPosition(x * 160 + 80, y * 160 + 80); //Finds the center as the scale is 160 and the center scale/2 because square

    //Replace the tile if any present
    for (size_t i = 0; i < tilesList.size(); ++i)
    {
        if (tilesList[i].GetPosition() == sprite.getPosition())
        {
            //o(1) delete but changes the order of tiles, 
            //but that doesn't matter cause you can draw the tiles in any order and it won't affect our game
            tilesList[i] = tilesList.back();
            tilesList.pop_back();
            break;
        }
    }

	tilesList.emplace_back(Entity::PhysicsProperties::Type::Static);
    Entity& newTile = tilesList.back();
    newTile.SetSprite(sprite);
	newTile.SetRectangle(160.0f, 160.0f);

	ConstructPath();

}

//Delete Tile at Position during Level Editor Mode
void Game::DeleteTileAtPosition(const sf::Vector2f& position)
{
    TileOption::TileType tileType = tileOptions[tileOptionIndex].GetTileType();
    if (tileType == TileOption::TileType::Null)
		return;

    std::vector<Entity>& tilesList = GetTileList(tileType);

    int x = position.x / 160;
    int y = position.y / 160;

    sf::Vector2f tilePosition(x * 160 + 80, y * 160 + 80);

    for (size_t i = 0; i < tilesList.size(); i++)
    {
        if (tilesList[i].GetPosition() == tilePosition)
        {
            tilesList[i] = tilesList.back();
            tilesList.pop_back();
            break;
        }
    }
}

std::vector<Entity>& Game::GetTileList(TileOption::TileType type)
{
    switch (type)
    {
    case TileOption::TileType::Aesthetic: return collisionTiles;
    case TileOption::TileType::Spawn: return spawnTiles;
    case TileOption::TileType::End: return endTiles;
    case TileOption::TileType::Path: return pathTiles;
	default: return collisionTiles;
    }
}

void Game::VisitNeighbourInPath(Path path, const sf::Vector2i& endCoords)
{
    const sf::Vector2i vCurrentTilePosition = path.back().currentTile->GetClosestCoordinates();

	//Store all the neighbouring tile coordinates
    const sf::Vector2i vNorthCoords(vCurrentTilePosition.x, vCurrentTilePosition.y - 1);
    const sf::Vector2i vEastCoords(vCurrentTilePosition.x + 1, vCurrentTilePosition.y);
    const sf::Vector2i vSouthCoords(vCurrentTilePosition.x, vCurrentTilePosition.y + 1);
    const sf::Vector2i vWestCoords(vCurrentTilePosition.x - 1, vCurrentTilePosition.y);

	//Recursive base case - if any of the neighbouring tiles are the end tile, then the path has been found, so return it
    if (endCoords == vNorthCoords || endCoords == vEastCoords || endCoords == vSouthCoords || endCoords == vWestCoords)
    {
		//Connect the current tile to the end tile and store the end tile in the path
        path.back().nextTile = &endTiles[0];
        path.emplace_back();
		PathTile& newTile = path.back();
        newTile.currentTile = &endTiles[0];
        paths.push_back(path);

        return;
    }

    const std::vector<Entity>& pathTiles = GetTileList(TileOption::TileType::Path);

    for (const Entity& pathTile : pathTiles)
    {
        const sf::Vector2i vPathTileCoords = pathTile.GetClosestCoordinates();

        //If the path already contains this tile, then skip (Not very efficient but it works) //REFACTOR LATER
        if (DoesPathContainCoordinates(path, vPathTileCoords))
        {
            continue;
        }

		//If the tile is the neighbour of the current tile then add it to the path and visit its neighbours
        if (vPathTileCoords == vNorthCoords || vPathTileCoords == vEastCoords || vPathTileCoords == vSouthCoords || vPathTileCoords == vWestCoords)
        {
            //Copy of path 
            Path newPath = path;
            newPath.back().nextTile = &pathTile;
			newPath.emplace_back();
			PathTile& newTile = newPath.back();
            newTile.currentTile = &pathTile;

            VisitNeighbourInPath(newPath, endCoords);
        }
    }
}

bool Game::DoesPathContainCoordinates(const Path& path, const sf::Vector2i& coordinates)
{
    for (const PathTile& pathTile : path)
    {
        if (pathTile.currentTile->GetClosestCoordinates() == coordinates)
        {
            return true;
        }
    }

    return false;
}
