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
 *  Text type of surface.
 *
 */

#include "SurfaceText.h"
#include "../Error/debugFunctions.h"

SurfaceText::SurfaceText() {
#ifdef DEBUG_MODE_ON
  DEB_ALLOCATED_TEXT++;
#endif  // DEBUG_MODE_ON
}

SurfaceText::SurfaceText(sf::Text t) : Text(t) {
#ifdef DEBUG_MODE_ON
  DEB_ALLOCATED_TEXT++;
#endif  // DEBUG_MODE_ON
}

SurfaceText::~SurfaceText() {
#ifdef DEBUG_MODE_ON
  DEB_ALLOCATED_TEXT--;
#endif  // DEBUG_MODE_ON
}