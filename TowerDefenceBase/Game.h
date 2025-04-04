#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "TileOption.h"

class Game
{
public:
	Game();
	~Game();

	enum GameMode{
		Play,
		LevelEditor
	};

	enum ScrollWheelInput{
		None,
		ScrollUp,
		ScrollDown
	};
	
	struct PathTile
	{
		const Entity* currentTile;
		const Entity* nextTile;
	};

	void Tick();

private:
	void UpdatePlayMode();
	void UpdateLevelEditorMode();
	void UpdatePhysics();

private:
	void ProcessCollision(Entity& entity, Entity& otherEntity);

	void Draw();
	void DrawLevelEditor();

	void HandleInput();
	void HandlePlayerInput();
	void HandleLevelEditorInput();

	void ConstructPath();

	void CreateTileAtPosition(const sf::Vector2f& position);
	void DeleteTileAtPosition(const sf::Vector2f& position);

	std::vector<Entity>& GetTileList(TileOption::TileType type);


private:
	sf::RenderWindow gameWindow;
	sf::Time deltaTime;
	GameMode gameMode;
	sf::Text gameModeText;
	sf::Font defaultFont;

	//Play Mode Variables
	//Sprites
	Entity player;
	Entity enemyBlueprint;
	Entity weapon;

	std::vector<Entity> enemies;

	//Textures 
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture weaponTexture;

	sf::Vector2f requestedMovementDirection;
	float playerSpeed;


	//Level Editor Mode Variables
	ScrollWheelInput scrollWheelInput;
	int tileOptionIndex;

	sf::Texture tileMapTexture;

	//Refactor to entities
	std::vector <TileOption> tileOptions;
	std::vector<Entity> collisionTiles;
	std::vector<Entity> spawnTiles;
	std::vector<Entity> endTiles;
	std::vector<Entity> pathTiles;

private: 
	//Pathfinding Variables and Methods
	typedef std::vector<PathTile> Path;

	void VisitNeighbourInPath(Path path, const sf::Vector2i& endCoords);
	bool DoesPathContainCoordinates(const Path& path, const sf::Vector2i& coordinates);

	std::vector<Path> paths;
};

