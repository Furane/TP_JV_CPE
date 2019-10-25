// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>

class EventReceiver : public irr::IEventReceiver
{
  irr::gui::IGUIEnvironment *gui;
  irr::scene::ISceneNode *node;
  bool button_pressed;
  int  old_x, old_y;
  std::vector<irr::video::ITexture*> textures;
  int current_texture;

  bool keyboard(const irr::SEvent &event);
  bool mouse(const irr::SEvent &event);
  bool gui_handler(const irr::SEvent &event);
public:
  EventReceiver();
  bool OnEvent(const irr::SEvent &event);
  void set_gui(irr::gui::IGUIEnvironment *gui);
  void set_node(irr::scene::ISceneNode *node);
  void set_textures(const std::vector<irr::video::ITexture *> &tex){textures=tex;}
};

#endif
