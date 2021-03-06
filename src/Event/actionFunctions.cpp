#include "actionFunctions.h"
#include "../Battle/B_Event/B_EventAttack.h"
#include "../Database/DatabaseJson.h"
#include "../Error/InitializationException.h"
#include "../Error/ValueException.h"
#include "../Menu/MenuBattle.h"
#include "../Menu/MenuChoice.h"
#include "../Menu/MenuDialogBox.h"
#include "../Menu/MenuInventory.h"
#include "../Menu/MenuSpells.h"
#include "../Menu/MenuTeam.h"
#include "../constants.h"

int af_exit(Game &g) {
  g.exit();

  return 1;
}

// Camera movement
int af_camUp(Game &g) {
  g.getRealMap().gotUpdated();
  return g.getRealMap().getRealCamera().move(0, -CAMERA_MOVING_Y);
}

int af_camDown(Game &g) {
  g.getRealMap().gotUpdated();
  return g.getRealMap().getRealCamera().move(0, CAMERA_MOVING_Y);
}

int af_camLeft(Game &g) {
  g.getRealMap().gotUpdated();
  return g.getRealMap().getRealCamera().move(-CAMERA_MOVING_X, 0);
}

int af_camRight(Game &g) {
  g.getRealMap().gotUpdated();
  return g.getRealMap().getRealCamera().move(CAMERA_MOVING_X, 0);
}

// Team movement
int af_teamUp(Game &g) {
  g.getRealMap().gotUpdated();
  return g.getRealTeam().move(0, -TEAM_MOVING_NORMAL_SPEED_Y,
                              Team::e_teamDirection::UP);
}

int af_teamDown(Game &g) {
  g.getRealMap().gotUpdated();
  return g.getRealTeam().move(0, TEAM_MOVING_NORMAL_SPEED_Y,
                              Team::e_teamDirection::DOWN);
}

int af_teamLeft(Game &g) {
  g.getRealMap().gotUpdated();
  return g.getRealTeam().move(-TEAM_MOVING_NORMAL_SPEED_X, 0,
                              Team::e_teamDirection::LEFT);
}
//#include <iostream>
int af_teamRight(Game &g) {
  g.getRealMap().gotUpdated();
  // std::cout << "\t\tRight pressed\n";
  return g.getRealTeam().move(TEAM_MOVING_NORMAL_SPEED_X, 0,
                              Team::e_teamDirection::RIGHT);
}

int af_teamStop(Game &g) { return g.getRealTeam().stopMoving(); }

int af_teamStopUp(Game &g) { return g.getRealTeam().stopMoving(Team::UP); }

int af_teamStopDown(Game &g) { return g.getRealTeam().stopMoving(Team::DOWN); }

int af_teamStopRight(Game &g) {
  return g.getRealTeam().stopMoving(Team::RIGHT);
}

int af_teamStopLeft(Game &g) { return g.getRealTeam().stopMoving(Team::LEFT); }

// Menu changement
int af_menuOpenSimple(Game &g) {
  g.getRealMap().gotUpdated();
  af_teamStop(g);
  g.getRealMenus().push_back(std::make_shared<MenuChoice>(
      g.getRealRessourceManager().at(Game::e_ressourcesLayer::RESSOURCES_MENU),
      g.getRealSurfaceManager(LAYER_MENU),
      g.getEventManager().getKeyEventLayer()));
  g.getRealEventManager().setKeyEventLayer(1);

  return std::dynamic_pointer_cast<MenuChoice>(g.getRealMenus().back())
      ->loadFromFile(g.getRealRessourceManager().at(
                         Game::e_ressourcesLayer::RESSOURCES_MENU),
                     g.getRealSurfaceManager(LAYER_MENU), MENU_SIMPLE_TEXT,
                     g.getMap().getCamera().getX(),
                     g.getMap().getCamera().getY());
}

int af_menuCloseSimple(Game &g) {
  g.getRealMap().gotUpdated();

  g.getRealSoundManager().addSound(
      g.getRealRessourceManager()
          .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
          .getTheSoundBuffer(2));

  return g.stopMenu();
}

int af_menuChoiceUp(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No menu");
  int ret = std::dynamic_pointer_cast<MenuChoice>(g.getRealMenus().back())
                ->setSelectedChoice(std::dynamic_pointer_cast<MenuChoice>(
                                        g.getRealMenus().back())
                                        ->getSelectedChoice() -
                                    1);
  if (ret > 0) {
    g.getRealMap().gotUpdated();
    g.getRealSoundManager().addSound(
        g.getRealRessourceManager()
            .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
            .getTheSoundBuffer(0));
  }
  return 0;
}

int af_menuChoiceDown(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No menu");
  int ret = std::dynamic_pointer_cast<MenuChoice>(g.getRealMenus().back())
                ->setSelectedChoice(std::dynamic_pointer_cast<MenuChoice>(
                                        g.getRealMenus().back())
                                        ->getSelectedChoice() +
                                    1);
  if (ret > 0) {
    g.getRealMap().gotUpdated();
    g.getRealSoundManager().addSound(
        g.getRealRessourceManager()
            .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
            .getTheSoundBuffer(0));
  }
  return 0;
}

int af_menuChoiceAction(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No menu");

  g.getRealSoundManager().addSound(
      g.getRealRessourceManager()
          .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
          .getTheSoundBuffer(1));

  return (std::dynamic_pointer_cast<MenuChoice>(g.getRealMenus().back()))
      ->getChoices()
      .at(std::dynamic_pointer_cast<MenuChoice>(g.getRealMenus().back())
              ->getSelectedChoice())
      ->getAction()
      .getAction()(g);
}

// Bag
int af_bagOpenComplete(Game &g) {
  g.getRealMap().gotUpdated();
  af_teamStop(g);
  g.getRealMenus().push_back(std::shared_ptr<MenuChoice>(new MenuChoice(
      g.getRealRessourceManager().at(Game::e_ressourcesLayer::RESSOURCES_MENU),
      g.getRealSurfaceManager(LAYER_MENU),
      g.getEventManager().getKeyEventLayer())));
  g.getRealEventManager().setKeyEventLayer(1);

  return std::dynamic_pointer_cast<MenuChoice>(g.getRealMenus().back())
      ->loadFromFile(g.getRealRessourceManager().at(
                         Game::e_ressourcesLayer::RESSOURCES_MENU),
                     g.getRealSurfaceManager(LAYER_MENU), MENU_BAG_TEXT,
                     g.getMap().getCamera().getX(),
                     g.getMap().getCamera().getY());
}

// DialogBox
int af_dialogBoxOpen(Game &g) {
  g.getRealMap().gotUpdated();
  af_teamStop(g);
  g.getRealMenus().push_back(std::shared_ptr<MenuDialogBox>(new MenuDialogBox(
      g.getRealRessourceManager().at(Game::e_ressourcesLayer::RESSOURCES_MENU),
      g.getRealSurfaceManager(LAYER_MENU),
      g.getEventManager().getKeyEventLayer())));
  g.getRealEventManager().setKeyEventLayer(2);

  return std::dynamic_pointer_cast<MenuDialogBox>(g.getRealMenus().back())
      ->load(g.getRealRessourceManager().at(
                 Game::e_ressourcesLayer::RESSOURCES_MENU),
             g.getRealSurfaceManager(LAYER_MENU),
             "Test this stupid dialog box.\\  And try to switch line or "
             "so\\And a new line\\This line is a veeeeeeeeeeeeeery long one "
             "because i need to put it on two lines so there is a lot of words "
             "there and most of them are useless\\Not the last\\And a last one",
             g.getMap().getCamera().getX(), g.getMap().getCamera().getY());
}

int af_dialogBoxContinue(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No dialog box");

  g.getRealMap().gotUpdated();
  g.getRealSoundManager().addSound(
      g.getRealRessourceManager()
          .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
          .getTheSoundBuffer(1));

  int ret = std::dynamic_pointer_cast<MenuDialogBox>(g.getRealMenus().back())
                ->continueText(g.getRealRessourceManager().at(
                                   Game::e_ressourcesLayer::RESSOURCES_MENU),
                               g.getRealSurfaceManager(LAYER_MENU));

  if (ret != 0) return af_menuCloseSimple(g);

  return ret;
}

// Spells
int af_menuSpellsOpen(Game &g, std::vector<Spell> sp) {
  g.getRealMap().gotUpdated();
  af_teamStop(g);
  /*std::vector<Spell> sp;

  if (g.getRealBattle().getStarted())
  {
          sp = g.getRealBattle().getSelectedTeamMate().getAllAvailableSpells();
  }
  else
  {
          ///return -1;
          sp.push_back(g.getRealDatabaseJson().getSpell(1));
          sp.push_back(g.getRealDatabaseJson().getSpell(2));
          sp.push_back(g.getRealDatabaseJson().getSpell(1));
          ///
  }*/

  if (sp.empty()) {
    if (g.getRealBattle().getStarted()) {
      std::dynamic_pointer_cast<MenuDialogBox>(
          g.getRealBattle().getRealBattleMenu().getRealMenus().at(
              MenuBattle::BM_DIALOG))
          ->addText("Aucun sort disponible");
      std::dynamic_pointer_cast<MenuDialogBox>(
          g.getRealBattle().getRealBattleMenu().getRealMenus().at(
              MenuBattle::BM_DIALOG))
          ->continueText(g.getRealRessourceManager(
                             Game::e_ressourcesLayer::RESSOURCES_MENU),
                         g.getRealSurfaceManager(LAYER_MENU));
    }

    return -1;
  }

  g.getRealMenus().push_back(std::make_shared<MenuSpells>(
      g.getRealRessourceManager().at(Game::e_ressourcesLayer::RESSOURCES_MENU),
      g.getRealSurfaceManager(LAYER_MENU),
      g.getEventManager().getKeyEventLayer()));
  g.getRealEventManager().setKeyEventLayer(3);

  if (g.getRealBattle().getStarted()) g.getRealBattle().setShown(false);

  return std::dynamic_pointer_cast<MenuSpells>(g.getRealMenus().back())
      ->load(g.getRealRessourceManager().at(
                 Game::e_ressourcesLayer::RESSOURCES_MENU),
             g.getRealSurfaceManager(LAYER_MENU), g.getMap().getCamera().getX(),
             g.getMap().getCamera().getY(), sp);
}

int af_menuSpellsClose(Game &g) {
  g.getRealMap().gotUpdated();

  g.getRealSoundManager().addSound(
      g.getRealRessourceManager()
          .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
          .getTheSoundBuffer(2));

  if (g.getRealBattle().getStarted()) {
    g.getRealBattle().setShown(true);
  }

  return g.stopMenu();
}

int af_menuSpellUp(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No menu");
  int ret = std::dynamic_pointer_cast<MenuSpells>(g.getRealMenus().back())
                ->getRealMenuChoices()
                .setSelectedChoice(std::dynamic_pointer_cast<MenuSpells>(
                                       g.getRealMenus().back())
                                       ->getRealMenuChoices()
                                       .getSelectedChoice() -
                                   1);
  if (ret > 0) {
    g.getRealMap().gotUpdated();
    g.getRealSoundManager().addSound(
        g.getRealRessourceManager()
            .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
            .getTheSoundBuffer(0));

    ret += std::dynamic_pointer_cast<MenuSpells>(g.getRealMenus().back())
               ->getRealDialogBox()
               .addText(std::dynamic_pointer_cast<MenuSpells>(
                            g.getRealMenus().back())
                            ->getSpells()
                            .at(std::dynamic_pointer_cast<MenuSpells>(
                                    g.getRealMenus().back())
                                    ->getRealMenuChoices()
                                    .getSelectedChoice())
                            .getDescription());
    ret += std::dynamic_pointer_cast<MenuSpells>(g.getRealMenus().back())
               ->getRealDialogBox()
               .continueText(g.getRealRessourceManager().at(
                                 Game::e_ressourcesLayer::RESSOURCES_MENU),
                             g.getRealSurfaceManager(LAYER_MENU));
  }
  return ret;
}

int af_menuSpellDown(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No menu");
  int ret = std::dynamic_pointer_cast<MenuSpells>(g.getRealMenus().back())
                ->getRealMenuChoices()
                .setSelectedChoice(std::dynamic_pointer_cast<MenuSpells>(
                                       g.getRealMenus().back())
                                       ->getRealMenuChoices()
                                       .getSelectedChoice() +
                                   1);

  if (ret > 0) {
    g.getRealMap().gotUpdated();
    g.getRealSoundManager().addSound(
        g.getRealRessourceManager()
            .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
            .getTheSoundBuffer(0));

    ret += std::dynamic_pointer_cast<MenuSpells>(g.getRealMenus().back())
               ->getRealDialogBox()
               .addText(std::dynamic_pointer_cast<MenuSpells>(
                            g.getRealMenus().back())
                            ->getSpells()
                            .at(std::dynamic_pointer_cast<MenuSpells>(
                                    g.getRealMenus().back())
                                    ->getRealMenuChoices()
                                    .getSelectedChoice())
                            .getDescription());
    ret += std::dynamic_pointer_cast<MenuSpells>(g.getRealMenus().back())
               ->getRealDialogBox()
               .continueText(g.getRealRessourceManager().at(
                                 Game::e_ressourcesLayer::RESSOURCES_MENU),
                             g.getRealSurfaceManager(LAYER_MENU));
  }
  return ret;
}

// Inventory
int af_menuInventoryOpen(Game &g) {
  g.getRealMap().gotUpdated();
  af_teamStop(g);
  g.getRealMenus().push_back(std::make_shared<MenuInventory>(
      g.getRealRessourceManager().at(Game::e_ressourcesLayer::RESSOURCES_MENU),
      g.getRealSurfaceManager(LAYER_MENU),
      g.getEventManager().getKeyEventLayer()));
  g.getRealEventManager().setKeyEventLayer(6);

  bool enabled[] = {false, false, true, true, true, true, false};

  if (g.getRealBattle().getStarted()) g.getRealBattle().setShown(false);

  return std::dynamic_pointer_cast<MenuInventory>(g.getRealMenus().back())
      ->load(g.getRealRessourceManager().at(
                 Game::e_ressourcesLayer::RESSOURCES_MENU),
             g.getRealSurfaceManager(LAYER_MENU), g.getMap().getCamera().getX(),
             g.getMap().getCamera().getY(),
             g.getRealTeam().getRealInventory().getRealItems(),
             std::vector<bool>(std::begin(enabled), std::end(enabled)));
}

int af_menuInventoryClose(Game &g) {
  g.getRealMap().gotUpdated();

  g.getRealSoundManager().addSound(
      g.getRealRessourceManager()
          .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
          .getTheSoundBuffer(2));

  if (g.getRealBattle().getStarted()) g.getRealBattle().setShown(true);

  return g.stopMenu();
}

int af_menuInventoryUp(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No menu");
  int ret = std::dynamic_pointer_cast<MenuInventory>(g.getRealMenus().back())
                ->getRealMenuChoices()
                .setSelectedChoice(std::dynamic_pointer_cast<MenuInventory>(
                                       g.getRealMenus().back())
                                       ->getRealMenuChoices()
                                       .getSelectedChoice() -
                                   1);
  if (ret > 0) {
    g.getRealMap().gotUpdated();
    g.getRealSoundManager().addSound(
        g.getRealRessourceManager()
            .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
            .getTheSoundBuffer(0));

    ret += std::dynamic_pointer_cast<MenuInventory>(g.getRealMenus().back())
               ->getRealDialogBox()
               .addText(std::dynamic_pointer_cast<MenuInventory>(
                            g.getRealMenus().back())
                            ->getItems()
                            .at(std::dynamic_pointer_cast<MenuInventory>(
                                    g.getRealMenus().back())
                                    ->getRealMenuChoices()
                                    .getSelectedChoice())
                            .first.getDescription());
    ret += std::dynamic_pointer_cast<MenuInventory>(g.getRealMenus().back())
               ->getRealDialogBox()
               .continueText(g.getRealRessourceManager().at(
                                 Game::e_ressourcesLayer::RESSOURCES_MENU),
                             g.getRealSurfaceManager(LAYER_MENU));
  }
  return ret;
}

int af_menuInventoryDown(Game &g) {
  if (g.getRealMenus().size() == 0) THROW_INIT("No menu");
  int ret = std::dynamic_pointer_cast<MenuInventory>(g.getRealMenus().back())
                ->getRealMenuChoices()
                .setSelectedChoice(std::dynamic_pointer_cast<MenuInventory>(
                                       g.getRealMenus().back())
                                       ->getRealMenuChoices()
                                       .getSelectedChoice() +
                                   1);

  if (ret > 0) {
    g.getRealMap().gotUpdated();
    g.getRealSoundManager().addSound(
        g.getRealRessourceManager()
            .at(Game::e_ressourcesLayer::RESSOURCES_MENU)
            .getTheSoundBuffer(0));

    ret += std::dynamic_pointer_cast<MenuInventory>(g.getRealMenus().back())
               ->getRealDialogBox()
               .addText(std::dynamic_pointer_cast<MenuInventory>(
                            g.getRealMenus().back())
                            ->getItems()
                            .at(std::dynamic_pointer_cast<MenuInventory>(
                                    g.getRealMenus().back())
                                    ->getRealMenuChoices()
                                    .getSelectedChoice())
                            .first.getDescription());
    ret += std::dynamic_pointer_cast<MenuInventory>(g.getRealMenus().back())
               ->getRealDialogBox()
               .continueText(g.getRealRessourceManager().at(
                                 Game::e_ressourcesLayer::RESSOURCES_MENU),
                             g.getRealSurfaceManager(LAYER_MENU));
  }
  return ret;
}

// Team Menu
int af_menuTeamOpenSpell(Game &g) {
  g.getRealMap().gotUpdated();
  af_teamStop(g);
  g.getRealMenus().push_back(std::make_shared<MenuTeam>(
      g.getRealRessourceManager().at(Game::e_ressourcesLayer::RESSOURCES_MENU),
      g.getRealSurfaceManager(LAYER_MENU),
      g.getEventManager().getKeyEventLayer()));
  g.getRealEventManager().setKeyEventLayer(7);

  return std::dynamic_pointer_cast<MenuTeam>(g.getRealMenus().back())
      ->load(MenuTeam::e_MenuTeamType::MENU_TEAM_SPELLS,
             g.getRealTeam().getRealTeamBattle(),
             g.getRealRessourceManager().at(
                 Game::e_ressourcesLayer::RESSOURCES_MENU),
             g.getRealSurfaceManager(LAYER_MENU), g.getMap().getCamera().getX(),
             g.getMap().getCamera().getY());
}

int af_menuTeamOpenChar(Game &g) {
  g.getRealMap().gotUpdated();
  af_teamStop(g);
  g.getRealMenus().push_back(std::make_shared<MenuTeam>(
      g.getRealRessourceManager().at(Game::e_ressourcesLayer::RESSOURCES_MENU),
      g.getRealSurfaceManager(LAYER_MENU),
      g.getEventManager().getKeyEventLayer()));
  g.getRealEventManager().setKeyEventLayer(7);

  return std::dynamic_pointer_cast<MenuTeam>(g.getRealMenus().back())
      ->load(MenuTeam::e_MenuTeamType::MENU_TEAM_CHAR,
             g.getRealTeam().getRealTeamBattle(),
             g.getRealRessourceManager().at(
                 Game::e_ressourcesLayer::RESSOURCES_MENU),
             g.getRealSurfaceManager(LAYER_MENU), g.getMap().getCamera().getX(),
             g.getMap().getCamera().getY());
}

int af_menuTeamClose(Game &g) { return af_menuCloseSimple(g); }

// Battle
/// General
int af_battleStart(Game &g) {
  af_teamStop(g);

  g.getRealBattle().start("data/graphic/battleback/001-grl01.jpg", TeamBattle(),
                          "", &g);

  g.getRealEventManager().setKeyEventLayer(BATTLE_KEY_EVENT_LAYER);

  return 0;
}

int af_battleEnd(Game &g) {
  // [TODO]

  g.getRealEventManager().setKeyEventLayer(0);

  g.getRealMap().gotUpdated();

  return g.getRealBattle().end(&g);
}

/// Menu Battle
int af_menuBattleDown(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().af_MenuBattleDown(g);
}

int af_menuBattleUp(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().af_MenuBattleUp(g);
}

int af_menuBattleRight(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().af_MenuBattleRight(g);
}

int af_menuBattleLeft(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().af_MenuBattleLeft(g);
}

int af_menuBattleEnter(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().af_MenuBattleEnter(g);
}

int af_menuBattleUseRight(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().setActiveMenu(
      MenuBattle::BM_RIGHT);
}

int af_menuBattleUseLeft(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().setActiveMenu(
      MenuBattle::BM_LEFT);
}

int af_menuBattleUseDialogBox(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().setActiveMenu(
      MenuBattle::BM_DIALOG);
}

int af_menuBattleUseChoice(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");
  return g.getRealBattle().getRealBattleMenu().setActiveMenu(
      MenuBattle::BM_CHOICE);
}

/// Event battle
int af_battleEventCreateAttack(Game &g) {
  if (!g.getRealBattle().getStarted()) THROW_INIT("No battle started");

  std::shared_ptr<B_EventAttack> evt = std::make_shared<B_EventAttack>(
      B_EventAttack(g.getRealBattle()
                        .getRealBattleOrder()
                        .at(g.getRealBattle().getBattleEventCreated() - 1)
                        ->getName() +
                    " attaque"));
  evt->getRealSound() = std::make_shared<sf::SoundBuffer>(
      g.getRealRessourceManager()
          .at(Game::e_ressourcesLayer::RESSOURCES_BATTLE)
          .getTheSoundBuffer(0));

  return g.getRealBattle().getRealBattleEventManager().createEvent(
      g.getRealBattle().getRealBattleOrder().at(
          g.getRealBattle().getBattleEventCreated() - 1),
      evt, g);
}