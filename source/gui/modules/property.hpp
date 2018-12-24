#pragma once

#include <memory>
#include <string>

class PropertiesPanel;

class Property
{
public:
  using UPtr = std::unique_ptr<Property>;

public:
  Property(PropertiesPanel& panel, std::string name) : mPanel(panel), mName{std::move(name)} {}
  virtual ~Property() = default;

  PropertiesPanel& getPanel() const { return mPanel; }
  std::string_view getName() const { return mName; }

private:
private:
  PropertiesPanel& mPanel;
  std::string mName;
};
