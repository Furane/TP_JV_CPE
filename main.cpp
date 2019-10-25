#include <irrlicht.h>

#include "events.h"
#include "gui_ids.h"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

/*===========================================================================*\
 * create_menu                                                               *
\*===========================================================================*/
static void create_menu(ig::IGUIEnvironment *gui)
{
  ig::IGUIContextMenu *submenu;

  // Les trois entrées principales :
  ig::IGUIContextMenu *menu = gui->addMenu();
  menu->addItem(L"File", -1, true, true);
  menu->addItem(L"Debug", -1, true, true);
  menu->addItem(L"Help", -1, true, true);

  // Le contenu du menu File :
  submenu = menu->getSubMenu(0);
  submenu->addItem(L"New game...", MENU_NEW_GAME);
  submenu->addSeparator();
  submenu->addItem(L"Quit", MENU_QUIT);

  // Le contenu du menu Debug :
  submenu = menu->getSubMenu(1);
  submenu->addItem(L"Bounding Box",   MENU_BOUNDING_BOX);
  submenu->addItem(L"Show Normals",   MENU_NORMALS);
  submenu->addItem(L"Show Triangles", MENU_TRIANGLES);
  submenu->addItem(L"Transparency",   MENU_TRANSPARENCY);

  // Le contenu du menu Help :
  submenu = menu->getSubMenu(2);
  submenu->addItem(L"About...", MENU_ABOUT);
}

/*===========================================================================*\
 * create_window                                                             *
\*===========================================================================*/
static void create_window(ig::IGUIEnvironment *gui)
{
  // La fenêtre
  ig::IGUIWindow *window = gui->addWindow(ic::rect<s32>(420,25, 620,460), false, L"Settings");

  // Une zone d'édition de texte, précédée d'un label
  gui->addStaticText(L"Value", ic::rect<s32>(22,48, 65,66), false, false, window);
  gui->addEditBox(L"1.0", ic::rect<s32>(65,46, 160,66), true, window, WINDOW_VALUE);

  // Un bouton à clicker
  gui->addButton(ic::rect<s32>(40,74, 140,92), window, WINDOW_BUTTON, L"Click me!");

  // Une case à cocher
  gui->addCheckBox(true, ic::rect<s32>(40,100, 140,118), window, WINDOW_CHECK_BOX, L"Select me!");

  // Une boite combo (une liste déroulante)
  gui->addStaticText(L"Choose one: ", ic::rect<s32>(22,126, 100,142), false, false, window);
  ig::IGUIComboBox *cbox = gui->addComboBox(ic::rect<s32>(100,126, 180,142), window, WINDOW_COMBO_BOX);
  cbox->addItem(L"Choice 1", WINDOW_COMBO_CHOICE_1);
  cbox->addItem(L"Choice 2", WINDOW_COMBO_CHOICE_2);
  cbox->addItem(L"Choice 3", WINDOW_COMBO_CHOICE_3);

  // Une liste déroulée
  gui->addStaticText(L"List:", ic::rect<s32>(22,150, 65,168), false, false, window);
  ig::IGUIListBox *lbox = gui->addListBox(ic::rect<s32>(40,170, 160,242), window, WINDOW_LIST_BOX, true);
  lbox->addItem(L"First Entry");
  lbox->addItem(L"Second Entry");
  lbox->addItem(L"Third Entry");

  // Une barre de défilement
  gui->addScrollBar(true, ic::rect<s32>(22,250, 160,268), window, WINDOW_SCROLLBAR);

  // Une spin box
  gui->addSpinBox(L"18.0", ic::rect<s32>(40,280, 160,298), true, window, WINDOW_SPIN_BOX);
} 

int main()
{
  // Le gestionnaire d'événements
  EventReceiver receiver;
  std::vector<iv::ITexture*> textures;
  
  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, &receiver);

  //Interface utilisateur
  ig::IGUIEnvironment *gui = device->getGUIEnvironment();
  
  iv::IVideoDriver *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");
  
  
  is::IAnimatedMeshSceneNode *perso_1 = smgr->addAnimatedMeshSceneNode(mesh);
  is::IAnimatedMeshSceneNode *perso_2 = smgr->addAnimatedMeshSceneNode(mesh);
  receiver.set_node(perso_1);
  
  textures.push_back(driver->getTexture("data/base.pcx"));
  textures.push_back(driver->getTexture("data/red_texture.pcx"));
  textures.push_back(driver->getTexture("data/blue_texture.pcx"));

  
  /*perso_1->setDebugDataVisible(is::EDS_FULL);is::EDS_BBOX |
                            is::EDS_NORMALS |
                            is::EDS_MESH_WIRE_OVERLAY |
                            is::EDS_HALF_TRANSPARENCY*/
  
  perso_1->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  perso_1->setMD2Animation(irr::scene::EMAT_SALUTE);//EMAT_STAND
  perso_1->setMaterialTexture(0, textures[0]);
  perso_1->setRotation(ic::vector3df(0, 90, 0));
  
  receiver.set_gui(gui);
  receiver.set_textures(textures);
  
  // Ajout d'un arbre billboard
  is::IBillboardSceneNode *billboard;
  billboard = smgr->addBillboardSceneNode(nullptr,
		                                      ic::dimension2d<f32>(50, 80),
		                                      ic::vector3df(0, 0, 50));
  billboard->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  billboard->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
  billboard->setMaterialTexture(0, driver->getTexture("data/tree.png"));

  perso_2->setMaterialFlag(iv::EMF_LIGHTING, false);
  perso_2->setMD2Animation(is::EMAT_RUN);
  perso_2->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
  is::ISceneNodeAnimator *anim =
            smgr->createFlyStraightAnimator(ic::vector3df(-100,0,60),
            ic::vector3df(100,0,60), 3500, true);
  perso_2->addAnimator(anim);

  // Ajout de l'archive qui contient entre autres un niveau complet
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");
  // On charge un bsp (un niveau) en particulier :
  is::IAnimatedMesh *decor = smgr->getMesh("20kdm2.bsp");
  is::IMeshSceneNode *node = smgr->addOctreeSceneNode(decor->getMesh(0));
  //Translation pour que nos personnages soient dans le décor
  node->setPosition(core::vector3df(-1350, -130, -1400));
  
  // Création du triangle selector
  scene::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(node->getMesh(), node);
  node->setTriangleSelector(selector);
  
  // Création de la caméra
  scene::ICameraSceneNode* camera =
    smgr->addCameraSceneNodeFPS(nullptr,
                                100,         // Vitesse de rotation
                                .3,          // Vitesse de déplacement
                                -1,          // Identifiant
                                nullptr, 0,  // Table de changement de touches
                                true,        // Pas de possibilité de voler
                                3);          // Vitesse saut
  camera->setPosition(ic::vector3df(50, 50, -60));
  camera->setTarget(ic::vector3df(-70, 30, -60));

  //smgr->addCameraSceneNodeFPS();
  //smgr->addCameraSceneNodeMaya();
  
  // Et l'animateur/collisionneur
  scene::ISceneNodeAnimator *colli;
  colli = smgr->createCollisionResponseAnimator(selector, 
                                               camera,  // Le noeud que l'on veut gérer
                                               ic::vector3df(30, 50, 30), // "rayons" de la caméra
                                               ic::vector3df(0, -10, 0),  // gravité
                                               ic::vector3df(0, 30, 0));  // décalage du centre
  camera->addAnimator(colli);
  
  // Création de notre Gui
  // Choix de la police de caractères
  ig::IGUISkin* skin = gui->getSkin();
  ig::IGUIFont* font = gui->getFont("data/fontlucida.png");
  skin->setFont(font);
  
  // La barre de menu
  create_menu(gui);

  // Une fenêtre pour différents réglages
  create_window(gui);

  // Chargement des textures pour les chiffres
  iv::ITexture *digits[10];
  digits[0] = driver->getTexture("data/0.png");
  digits[1] = driver->getTexture("data/1.png");
  digits[2] = driver->getTexture("data/2.png");
  digits[3] = driver->getTexture("data/3.png");
  digits[4] = driver->getTexture("data/4.png");
  digits[5] = driver->getTexture("data/5.png");
  digits[6] = driver->getTexture("data/6.png");
  digits[7] = driver->getTexture("data/7.png");
  digits[8] = driver->getTexture("data/8.png");
  digits[9] = driver->getTexture("data/9.png");

  // Création des places pour les chiffres
  ig::IGUIImage *score_10000 = gui->addImage(ic::rect<s32>(10,25,  50,65)); score_10000->setScaleImage(true);
  ig::IGUIImage *score_1000  = gui->addImage(ic::rect<s32>(50,25,  90,65)); score_1000->setScaleImage(true);
  ig::IGUIImage *score_100   = gui->addImage(ic::rect<s32>(90,25,  130,65)); score_100->setScaleImage(true);
  ig::IGUIImage *score_10    = gui->addImage(ic::rect<s32>(130,25, 170,65)); score_10->setScaleImage(true);
  ig::IGUIImage *score_1     = gui->addImage(ic::rect<s32>(170,25, 210,65)); score_1->setScaleImage(true);

  int score = 0;
  
  while(device->run())
  {
      driver->beginScene(true, true, iv::SColor(100,150,200,255));

      // Dessin de la scène :
      smgr->drawAll();
      
      // Calcul du score :
      score++; if (score == 50000) score = -1;
      // Mise à jour du score :
      score_10000->setImage(digits[(score / 10000) % 10]);
      score_1000->setImage(digits[(score / 1000) % 10]);
      score_100->setImage(digits[(score / 100) % 10]);
      score_10->setImage(digits[(score / 10) % 10]);
      score_1->setImage(digits[(score / 1) % 10]);
    
      gui->drawAll();
      //
      driver->endScene();
  }
  device->drop();
}
