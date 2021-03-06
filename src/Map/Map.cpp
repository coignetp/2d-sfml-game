/**
 *  @file
 *  @author  Paul Coignet
 *  @date    11 / 11 / 2017
 *  @version 0.1
 *
 *  @brief Class header.
 *
 *  @section DESCRIPTION
 *
 *  The map information.
 *
 */

#include "Map.h"
#include <memory>
#include "../Error/RessourceException.h"
#include "../Error/ValueException.h"
#include "../Game.h"
#include "../constants.h"
#include "ManageSurfaces.h"

using namespace sf;
using namespace std;

#include <iostream>

Map::Map(Game &g) : m_name("") {
  m_paused = false;
  m_updated = true;
  m_nbLayers = 0;
  m_width = 0;
  m_height = 0;

  setCamera(Camera(-20, 0, CAMERA_WIDTH, CAMERA_HEIGHT, WINDOW_WIDTH,
                   WINDOW_HEIGHT, true));

  Font f;
  Text t;
  f.loadFromFile(MAP_FONT_TEXT);
  t.setFont(*g.getRealRessourceManager()
                 .at(Game::e_ressourcesLayer::RESSOURCES_MAP)
                 .addFont(f));
  t.setPosition(0, 0);
  m_mapName = g.getRealSurfaceManager()
                  .at(LAYER_MAP_END)
                  .addSurface(ManageSurfaces::e_thing::TEXT,
                              std::make_shared<SurfaceText>(t));

  setName("No name");
}

Map::~Map() {}

// Return if the map is paused
bool Map::getPaused() const { return m_paused; }

// Return if the map was updated since the last print
bool Map::getUpdated() const { return m_updated; }

// Return the width of the layer
int Map::getWitdth() const { return m_width; }

// Return the height of the layer
int Map::getHeight() const { return m_height; }

// Return the name of the actual map
string Map::getName() const { return m_name; }

// Return the surface of the name
std::string Map::getTextName() const {
  return std::dynamic_pointer_cast<SurfaceText>(m_mapName->second)->getString();
}

// Return the font of sthe surface name
/*Font Map::getFont() const
{
        return m_fontNameText;
}*/

// Return all the layers
vector<Layer> Map::getLayers() const { return m_layers; }

// Return the number of layers
int Map::getNumberOfLayer() const { return m_nbLayers; }

// Return the tileset of this map
Texture Map::getTileset() const { return m_tileset; }

// Return the camera
Camera Map::getCamera() const { return m_camera; }

// Return the hitbox manager
ManageHitbox Map::getHitboxManager() const { return m_hitboxManager; }

// Return the camera with modifying possibilities
Camera &Map::getRealCamera() { return m_camera; }

// Return the hitbox manager with modifying possibilities
ManageHitbox &Map::getRealHitboxManager() { return m_hitboxManager; }

// Modify if the map is paused (won't be printed)
int Map::setPaused(const bool &b) {
  m_paused = b;

  return 0;
}

// Modify the width of the layer
int Map::setWitdth(const int &w) {
  if (w < 0) THROW_VALUE(std::to_string(w));

  m_width = w;

  return 0;
}

// Modify the height of the layer
int Map::setHeight(const int &h) {
  if (h < 0) THROW_VALUE(std::to_string(h));

  m_height = h;

  return 0;
}

// Modify the name of the actual map
int Map::setName(std::string name) {
  if (name.empty()) THROW_VALUE("Empty string");

  m_name = name;

  std::dynamic_pointer_cast<SurfaceText>(m_mapName->second)
      ->setString(std::string(name));
  std::dynamic_pointer_cast<SurfaceText>(m_mapName->second)
      ->setCharacterSize(MENU_SIMPLE_FONTSIZE);
  std::dynamic_pointer_cast<SurfaceText>(m_mapName->second)
      ->setStyle(sf::Text::Bold);
  m_mapName->second->setDimensions(
      m_camera.getX(), m_camera.getY(),
      int(std::dynamic_pointer_cast<SurfaceText>(m_mapName->second)
              ->getLocalBounds()
              .width),
      int(std::dynamic_pointer_cast<SurfaceText>(m_mapName->second)
              ->getLocalBounds()
              .height));

  return 0;
}

/*
// Modify the fint of the surface name
int Map::setFont(Font f)
{
        m_fontNameText = f;

        m_textName.setFont(m_fontNameText);

        return 0;
}*/

// Modify the tileset of this map
int Map::setTileset(std::string filename) {
  if (!m_tileset.loadFromFile(filename))
    THROW_RESSOURCE("Map tileset", filename);

  return 0;
}
int Map::setTileset(Texture t) {
  m_tileset = t;

  return 0;
}

// Modify the camera
int Map::setCamera(const Camera &cam) {
  m_camera = cam;
  return 0;
}

// Ask the map a new print
int Map::gotUpdated() {
  m_updated = true;
  return 0;
}

// Load a new map
int Map::load(std::string filename, Game &g) {
  // [TODO]

  // Open the file

  // Read the header
  setName("Test map");
  m_nbLayers = 3;
  m_width = 40;
  m_height = 40;
  m_camera.setMaxX(m_width * TILE_WIDTH);
  m_camera.setMaxY(m_height * TILE_HEIGHT);
  g.getRealSoundManager().addMusic("data/audio/bgm/001-Field01.flac");

  m_layers.resize(m_nbLayers);

  for (int i(0); i < m_nbLayers; ++i) {
    m_layers.at(i).resize(m_width, m_height);
    m_layers.at(i).setTileH(TILE_HEIGHT);
    m_layers.at(i).setTileW(TILE_WIDTH);
  }

  // Get the Tileset
  setTileset(string("data/graphic/tileset/001-Grassland01.png"));

  // Read the data
  for (int i(0); i < m_width; i++) {
    for (int j(0); j < m_height; j++) {
      m_layers.at(0).setTile(i, j, Tile(0, 0, 32, 32));
    }
  }

  m_layers.at(1).setTile(5, 5, Tile(32, 64, 32, 32));
  m_hitboxManager.addTileHitbox(5, 5, Hitbox(-1, 1, 1, 30, 30));

  m_updated = true;

  return 0;
}

// Print the map
int Map::print(RenderWindow &win, std::vector<ManageSurfaces> &ms) {
  if (!m_updated || m_paused) return 1;

  m_mapName->second->setX(m_camera.getX());
  m_mapName->second->setY(m_camera.getY());

  for (int i(0); i < m_nbLayers; i++) {
    m_layers.at(i).printLayer(m_tileset, win, m_camera.getX(), m_camera.getY(),
                              m_camera.getWidth(), m_camera.getHeight());
    ms.at(i).print(win, m_camera);
  }

  for (int i(m_nbLayers); i <= LAYER_MAP_END; i++) {
    ms.at(i).print(win, m_camera);
  }

  m_updated = false;
  return 0;
}

// Update every item of the map
int Map::update() {
  // [TODO]
  return 0;
}