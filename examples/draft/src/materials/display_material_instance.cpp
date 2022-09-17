#include "draft/materials/display_material_instance.h"

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "sol/material/forward/forward_material_manager.h"

DisplayMaterialInstance::DisplayMaterialInstance(sol::Texture2D& tex) : texture(&tex) {}

DisplayMaterialInstance::~DisplayMaterialInstance() noexcept = default;

uint32_t DisplayMaterialInstance::getSetIndex() const { return 0; }

bool DisplayMaterialInstance::isUniformBufferStale(size_t binding) const { return false; }

const void* DisplayMaterialInstance::getUniformBufferData(size_t binding) const { return nullptr; }

sol::Texture2D* DisplayMaterialInstance::getTextureData(size_t binding) const { return texture; }

void DisplayMaterialInstance::setTexture(sol::Texture2D& tex) { texture = &tex; }
