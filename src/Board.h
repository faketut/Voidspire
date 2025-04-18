#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <random>
#include "Enum.cc"
#include "Player.cc"
#include "Item.cc"
#include "Position.cc"
#include "Enemy.cc"

const int BOARD_WIDTH = 79;
const int BOARD_HEIGHT = 25;
const int POTION_CNT = 10;
const int GOLD_CNT = 10;
const int ENEMY_CNT = 20;

class Board; // Forward declaration for Board

class Tile {
private:
    Position pos;
    std::shared_ptr<Entity> entity; // Base class for Enemy, PlayerCharacter, Item
    Type t;
    std::weak_ptr<Board> board;
public:
    Tile(Position pos, std::shared_ptr<Entity> const& ent, Type type, std::shared_ptr<Board> const& b);
    Position getPosition() const;
    void setPosition(const Position& newPos);
    bool isPickable();
    int getX() const;
    int getY() const;
    bool near(const Position& other);
    Type getType() const;
    void setType(Type type);
    void setEntity(std::shared_ptr<Entity> newEntity);
    std::shared_ptr<Entity> getEntity() const;
};
using EntityPtr = std::shared_ptr<Entity>;
using TilePtr = std::shared_ptr<Tile>;
using TileGrid = std::vector<std::vector<TilePtr>>;
class Board : public std::enable_shared_from_this<Board>{
private:
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::vector<std::shared_ptr<Tile>> enemyTiles;
    std::shared_ptr<Tile> pt;
    bool compassPickedUp;          
    bool allMerchantsHostile;
    int floorId;
    std::string filename;
    std::string dialog;
    bool setSuit;
    int maxFloorId;

    // Private helper methods
    template<typename T>
    std::shared_ptr<T> safeCast(const EntityPtr& entity) const;
    void handleItemProtection(TilePtr tile, const Position& pos);
    int calculateDamage(const Tile &attacker, const Tile &defender);
    void handleItemPickup(const std::shared_ptr<Tile> &tile);
    void loadFromFile(int floorId);


public:
    // Constructors and Initialization
    Board(std::string filename = "./files/default.txt", int floorId = 0);
    void initFloor();
    // Setters
    void setCompassPickedUp(bool b);
    void setAllMerchantsHostile();
    void setFloorId(int i);
    void setTile(const Position& pos, Type t, std::shared_ptr<Entity> entity = std::shared_ptr<Entity>());
    void setMaxFloorId(std::string fn);
    void setBarrierSuit();
    // Getters
    std::shared_ptr<Tile> getPc() const;
    int getFloorId() const;
    bool isAllMerchantsHostile() const;
    bool isCompassPickedUp() const;
    int getMaxFloorId() const;
    bool isHostile(const Tile& t) const;
    bool getSetSuit()const;
    // Game Logic Processing
    void updateEnemies();
    void attackPc(Tile& et);
    void moveEnemy(Tile& t);
    void attackEnemy(Tile& et);
    void die(Tile& t);

    // Player Control
    void movePc(Direction dir);
    void pickUp(Direction dir);

    // Game Flow
    bool nextFloor();

    // Board Operations
    Position convertDirection(Tile& t, Direction dir);
    template<typename Func>
    void loadBoard(int floorId, std::string& filename,Func processChar);
    void initByFile(int floorId);
    void displayBoard() const;
    void printBoard() const;
    std::shared_ptr<Entity> createEntity(Type type, const Position& pos);

    // Data Query
    bool isValidMove(Tile& t, Position& pos) const;
    std::vector<std::shared_ptr<Tile>> getNeighbourTiles(const Position& pos,Type t) const;
    std::shared_ptr<Tile> getTile(const Position& pos) const;
    std::vector<std::shared_ptr<Tile>> getFloorTile() const;
    std::shared_ptr<Tile> getRandomTile(const std::vector<std::shared_ptr<Tile>>& tiles,size_t start = 0, size_t end = 0);
    std::vector<std::shared_ptr<Tile>> getNeighbourEnemyTilesExceptDragonAndMerchant(const Position& pos) const;
    std::vector<std::shared_ptr<Tile>> getAllEnemyTileExceptDragonAndMerchant() const;
    // Help Information
    std::string seeDiaHelp(Direction dir);
    std::string diaHelp();
    std::shared_ptr<Tile> findEnemyTile(const Tile& tile);
    void assignItemToDragon(const Position& pos, const std::shared_ptr<Item>& it);

    // Damage Calculation
    int calDamage(Tile& player, Tile& enemy);

    // Special Items Placement
    std::shared_ptr<Tile> placeDragonHoard(const std::shared_ptr<Item>& gold, const Position& pos);
    std::shared_ptr<Entity> getEntity(Type type, Position& pos);

    // Enemy Management
    void addEnemyTile(std::shared_ptr<Tile>& enemyTile);
    void removeEnemyTile(const std::shared_ptr<Tile>& tileToRemove);

    // File Operations
};
#endif
